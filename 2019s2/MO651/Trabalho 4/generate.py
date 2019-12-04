import matplotlib
import matplotlib.pyplot as plt
import time
from math import sin, cos, pi
import numpy as np
import cv2
import vrep
import warnings
import os
import os.path as osp
import argparse
from robot import Robot
warnings.filterwarnings("ignore")

# input
parser = argparse.ArgumentParser(description='Generate imgs and positions files')

# path files
parser.add_argument('--path-base', type=str, default='/home/vinicius/dataset')
parser.add_argument('--folder', type=str, default='scene01')
parser.add_argument('--positions-file', type=str, default='positions.txt')

# robot configuration
parser.add_argument('--time-min', type=int, default=3)

args = parser.parse_args()

# robot initialization
robot = Robot()

R = robot.WHEEL_RADIUS
L = robot.L
leftMotorHandle = robot.motors_handle['left']
rightMotorHandle = robot.motors_handle['right']
sonarHandle = robot.us_handle
laser2D = robot.laser_handle
clientID = robot.clientID
p3dx = robot.robot_handle

def mkdir_if_missing(directory):
    if not osp.exists(directory):
        try:
            os.makedirs(directory)
        except OSError as e:
            if e.errno != errno.EEXIST:
                raise

def correctEncAngle(angleEnc):
    if angleEnc >=0:
        return angleEnc
    else:
        return 2*pi+angleEnc

def main():
	idx_img = 0
	j = 0

	mkdir_if_missing(osp.join(args.path_base, args.folder))

	file = open(osp.join(args.path_base, args.folder, args.positions_file),'w')

	t_end = time.time() + 60 * args.time_min
	while time.time() < t_end:
		resolution, image = robot.read_vision_sensor()

		cv2.imwrite(osp.join(args.path_base, args.folder) + "/" + str(idx_img).zfill(6) + ".png", image)

		if j == 0:
			error_pos, pos = vrep.simxGetObjectPosition(clientID, p3dx, -1, vrep.simx_opmode_streaming)
			error_angle, angle = vrep.simxGetObjectOrientation(clientID, p3dx, -1, vrep.simx_opmode_streaming)
			# Primeira posição do robô
			E = [pos[0],pos[1],angle[2]]
			errorEncL, iniLeftAngle = vrep.simxGetJointPosition(clientID, leftMotorHandle, vrep.simx_opmode_streaming)
			errorEncR, iniRightAngle = vrep.simxGetJointPosition(clientID, rightMotorHandle, vrep.simx_opmode_streaming)
			start_time = time.time()
			Vl = 1
			Vr = 1
		else:
			error_pos, pos = vrep.simxGetObjectPosition(clientID, p3dx, -1, vrep.simx_opmode_streaming)
			error_angle, angle = vrep.simxGetObjectOrientation(clientID, p3dx, -1, vrep.simx_opmode_streaming)

			angleEncLeft = vrep.simxGetJointPosition(clientID, leftMotorHandle, vrep.simx_opmode_streaming)[1]
			angleEncRight = vrep.simxGetJointPosition(clientID, rightMotorHandle, vrep.simx_opmode_streaming)[1]
			t = time.time() - start_time

			start_time = time.time()
			angleEncLeft = correctEncAngle(angleEncLeft)
			angleEncRight = correctEncAngle(angleEncRight)

			if 0 <= angleEncLeft <= pi and pi <= iniLeftAngle <= 2*pi or angleEncLeft > iniLeftAngle:
				leftDiff = pi - abs(abs(angleEncLeft - iniLeftAngle) - pi)
			else:
				leftDiff = -(pi - abs(abs(angleEncLeft - iniLeftAngle) - pi))

			if 0 <= angleEncRight <= pi and pi <= iniRightAngle <= 2*pi or angleEncRight > iniRightAngle:
				rightDiff = pi - abs(abs(angleEncRight - iniRightAngle) - pi)
			else:
				rightDiff = -(pi - abs(abs(angleEncRight - iniRightAngle) - pi))
			# Velocidade angular das rodas
			Vl = R*leftDiff/t
			Vr = R*rightDiff/t

			# Variação de espaço
			S = ((Vr+Vl)/2)*t

			# Variação de orientação
			angT = ((Vr-Vl)/(L))*t

			iniLeftAngle = angleEncLeft
			iniRightAngle = angleEncRight

			E = np.add(E, [S*cos(E[2]+angT/2),S*sin(E[2]+angT/2),angT])

		j += 1

		file.write("{} {} {} {} {} {}\n".format(pos[0],pos[1],pos[2],E[0],E[1],E[2]))

		ultrassonic = robot.read_ultrassonic_sensors()

		aRi = robot.get_right_angle()
		aLi = robot.get_left_angle()
		# Calculo de um valor de distancia baseado nos sensores ultrassonicos frontais do robô
		value = (0.2*ultrassonic[2]) + ((0.3*ultrassonic[3]) + (0.3*ultrassonic[4])) + (0.2*ultrassonic[5])
		# se a distancia for maior que um valor de limiar, o robô pode andar para a frente com segurança
		if value > 3.0:
		    robot.set_left_velocity(2.0) #rad/s
		    robot.set_right_velocity(2.0)
		    time.sleep(2)
		else:
		    # Senão, o robô ira realizar uma mudança de direção para a esquerda ou direita,
		    # baseado nos valores dos sensores ultrassonicos das laterais
		    esq = ultrassonic[0]
		    dirr = ultrassonic[7]
		    if esq >= 5.0 or (esq >=dirr and esq> 3.0):
		        robot.set_left_velocity(-2.0) 
		        robot.set_right_velocity(2.0)
		        time.sleep(1)
		    elif dirr >= 5.0 or (dirr >= esq and dirr > 3.0):
		        robot.set_left_velocity(2.0) 
		        robot.set_right_velocity(-2.0)
		        time.sleep(1)
		    else:
		        robot.set_left_velocity(-2.0) 
		        robot.set_right_velocity(2.0)
		        time.sleep(2)
		robot.stop()

		idx_img += 1

if __name__ == '__main__':
	main()