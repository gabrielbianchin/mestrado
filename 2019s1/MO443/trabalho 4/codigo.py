import cv2
import sys
import numpy as np

def function_sift(img1, img2, img1color, img2color):
	#descritor sift
	sift = cv2.xfeatures2d.SIFT_create()

	#keypoints e descritores
	kp1, des1 = sift.detectAndCompute(img1, None)
	kp2, des2 = sift.detectAndCompute(img2, None)

	#seleciona os melhores matchings
	index_params = dict(algorithm = 1, trees = 5)
	search_params = dict(checks = 50)
	flann = cv2.FlannBasedMatcher(index_params, search_params)
	matches = flann.knnMatch(des1,des2,k=2)
	
	good = []
	for m,n in matches:
	    if m.distance < 0.75*n.distance:
	        good.append(m)

	pts1 = np.float32([kp1[m.queryIdx].pt for m in good]).reshape(-1,1,2)
	pts2 = np.float32([kp2[m.trainIdx].pt for m in good]).reshape(-1,1,2)

	h, status = cv2.findHomography(pts1, pts2, cv2.RANSAC)

	im_out = cv2.warpPerspective(img1color, h, (img2color.shape[1] + img1color.shape[1],img2color.shape[0]))

	cv2.imwrite('immm.jpg', im_out)

	#cv2.imshow('imagem', im_out)
	#cv2.waitKey(0)

	#M, mask = cv2.findHomography(kp1, kp2, cv2.RANSAC, 5.0)

	#print(mask)

	img = cv2.drawMatches(img1color, kp1, img2color, kp2, good[:10], None, flags=2)

	return img

def function_surf(img1, img2):
	#descritor surf
	surf = cv2.xfeatures2d.SURF_create()

	#keypoints e descritores
	kp1, des1 = surf.detectAndCompute(img1, None)
	kp2, des2 = surf.detectAndCompute(img2, None)


def function_orb(img1, img2):
	#descritor orb
	orb = cv2.ORB_create()

	#keypoints e descritores
	kp1, des1 = orb.detectAndCompute(img1, None)
	kp2, des2 = orb.detectAndCompute(img2, None)



#diretorio da imagem 1
dir_imagem1 = sys.argv[1]

#diretorio da imagem 2
dir_imagem2 = sys.argv[2]

#descritor escolhido
descritor = sys.argv[3]

#prefixo das imagens de saida
saida = sys.argv[4]

#abrindo as imagens em tons de cinza
img1 = cv2.imread(dir_imagem1, 0)
img1color = cv2.imread(dir_imagem1)
img2color = cv2.imread(dir_imagem2)
img2 = cv2.imread(dir_imagem2, 0)

#verifica o descritor escolhido
if descritor == 'sift':
	img = function_sift(img1, img2, img1color, img2color)
	cv2.imwrite('saida/' + saida + '-linhas.jpg', img)

elif descritor == 'surf':
	img = function_surf(img1, img2, img1color, img2color)

elif descritor == 'orb':
	img = function_orb(img1, img2)

else:
	print('Nao foi possivel aplicar o descritor')