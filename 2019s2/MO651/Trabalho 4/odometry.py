import numpy as np
import cv2
import timeit
import time
import math
import argparse
import os
import os.path as osp

# input
parser = argparse.ArgumentParser(description='Odometry and Visual Odometry informations')

parser.add_argument('--path-dataset', type=str, default='./dataset')
parser.add_argument('--scene', type=str, default='scene01')
parser.add_argument('--positions-file', type=str, default='positions.txt')
parser.add_argument('--descriptor', type=str, default='fast')

args = parser.parse_args()

config = {
	'scene01': {
		'rot': 210,
		'rot_odo': 0,
		'focal': 300,
		'max_frame': 113
	},
	'scene02': {
		'rot': 110,
		'rot_odo': 20,
		'focal': 300,
		'max_frame': 110
	},
	'scene03': {
		'rot': 120,
		'rot_odo': 25,
		'focal': 100,
		'max_frame': 49
	},
	'scene04': {
		'rot': 220,
		'rot_odo': 130,
		'focal': 700,
		'max_frame': 125
	},
	'scene05': {
		'rot': -90,
		'rot_odo': 180,
		'focal': 650,
		'max_frame': 55
	}
}

def getAbsoluteScale(f, frame_id):
		x_pre, y_pre, z_pre = f[frame_id-1][0], f[frame_id-1][1], f[frame_id-1][2]
		x    , y    , z     = f[frame_id][0], f[frame_id][1], f[frame_id][2]
		scale = np.sqrt((x-x_pre)**2 + (y-y_pre)**2 + (z-z_pre)**2)
		return x, y, z, scale

def featureTracking(img_1, img_2, p1):
	lk_params = dict(winSize = (21,21),
					maxLevel = 3,
					criteria = (cv2.TERM_CRITERIA_EPS | cv2.TERM_CRITERIA_COUNT, 30, 0.01))

	p2, st, err = cv2.calcOpticalFlowPyrLK(img_1, img_2, p1, None, **lk_params)
	st = st.reshape(st.shape[0])

	p1 = p1[st==1]
	p2 = p2[st==1]

	return p1,p2

def featureDetection(desc_name):
	if desc_name == 'fast':
		thresh = dict(threshold=25, nonmaxSuppression=True)
		detection = cv2.FastFeatureDetector_create(**thresh)
	elif desc_name == 'sift':
		detection = cv2.xfeatures2d.SIFT_create()
	return detection

def getTruePose(path_positions):
    return np.genfromtxt(path_positions, delimiter=' ',dtype=np.float64)

def getImages(path_folder, i):
    return cv2.imread(path_folder + '/{0:06d}.png'.format(i), 0)

def norm(v):
	return ((300.0-0.0)/(5.0-(-5.0))) * (v-(-5.0))

def rotate(origin, point, angle):
	ox, oy = origin
	px, py = point

	qx = ox + math.cos(angle) * (px - ox) - math.sin(angle) * (py - oy)
	qy = oy + math.sin(angle) * (px - ox) + math.cos(angle) * (py - oy)
	return int(qx), int(qy)

def main():
	ground_truth = getTruePose(osp.join(args.path_dataset, args.scene, args.positions_file))

	img_1 = getImages(osp.join(args.path_dataset, args.scene), 1)
	img_2 = getImages(osp.join(args.path_dataset, args.scene), 2)

	detector = featureDetection(args.descriptor)
	kp1 = detector.detect(img_1)
	p1 = np.array([ele.pt for ele in kp1],dtype='float32')
	p1, p2 = featureTracking(img_1, img_2, p1)

	fc = config[args.scene]['focal']
	pp = (128, 128)

	E, mask = cv2.findEssentialMat(p2, p1, fc, pp, cv2.RANSAC,0.999,1.0)
	_, R, t, mask = cv2.recoverPose(E, p2, p1,focal=fc, pp = pp)

	MAX_FRAME = config[args.scene]['max_frame']
	MIN_NUM_FEAT = 1500

	preFeature = p2
	preImage = img_2

	R_f = R
	t_f = t

	start = timeit.default_timer()

	traj = np.zeros((600, 600, 3), dtype=np.uint8)

	maxError = 0

	for numFrame in range(2, MAX_FRAME):
		print(numFrame)
		if (len(preFeature) < MIN_NUM_FEAT):
			feature = detector.detect(preImage)
			preFeature = np.array([ele.pt for ele in feature],dtype='float32')

		curImage = getImages(osp.join(args.path_dataset, args.scene), numFrame)

		kp1 = detector.detect(curImage)
		preFeature, curFeature = featureTracking(preImage, curImage, preFeature)
		E, mask = cv2.findEssentialMat(curFeature, preFeature, fc, pp, cv2.RANSAC,0.999,1.0)
		_, R, t, mask = cv2.recoverPose(E, curFeature, preFeature, focal=fc, pp = pp)

		truth_x, truth_y, truth_z, absolute_scale = getAbsoluteScale(ground_truth, numFrame)

		if absolute_scale > 0.00:  
			t_f = t_f + absolute_scale*R_f.dot(t)
			R_f = R.dot(R_f)

		preImage = curImage
		preFeature = curFeature

		draw_x, draw_y = t_f[0], t_f[2]
		draw_tx, draw_ty = int(norm(truth_x)), int(norm(truth_y))
		draw_odox, draw_odoy = int(norm(ground_truth[numFrame][3])), int(norm(ground_truth[numFrame][4]))

		draw_x = int(norm(-draw_x))
		draw_y = int(norm(-draw_y))

		if numFrame == 2:
			origem = (draw_tx, draw_ty)

		draw_x, draw_y = rotate(origem, (draw_x, draw_y), math.radians(config[args.scene]['rot']))
		draw_odox, draw_odoy = rotate(origem, (draw_odox, draw_odoy), math.radians(config[args.scene]['rot_odo']))

		if numFrame == 2:
			var_x = draw_tx - draw_x
			var_y = draw_ty - draw_y

			odo_x = draw_tx - draw_odox
			odo_y = draw_ty - draw_odoy
			
		curError = np.sqrt((t_f[0]-truth_x)**2 + (t_f[1]-truth_y)**2)

		if (curError > maxError):
			maxError = curError

		norm_x = 150
		norm_y = 100

		cv2.circle(traj, (draw_x+norm_x+var_x, draw_y+norm_y+var_y) ,1, (0,0,255), 2)
		cv2.circle(traj, (draw_tx+norm_x, draw_ty+norm_y) ,1, (255,0,0), 2)
		cv2.circle(traj, (draw_odox+norm_x+odo_x, draw_odoy+norm_y+odo_y) ,1, (0,255,0), 2)

		cv2.rectangle(traj, (10, 30), (550, 50), (0,0,0), cv2.FILLED)
		text = "Coordinates: x ={0:02f}m y = {1:02f}m z = {2:02f}m".format(float(t_f[0]), float(t_f[1]), float(t_f[2]))
		cv2.putText(traj, text, (10,50), cv2.FONT_HERSHEY_PLAIN, 1, (255,255,255), 1, 8)

		cv2.imshow("Trajectory", traj)
		cv2.imshow("Camera", curImage)
		k = cv2.waitKey(1) & 0xFF
		if k == 27:
			break

		time.sleep(0.05)

	cv2.imwrite("./" + args.scene + ".png", traj)

if __name__ == '__main__':
	main()