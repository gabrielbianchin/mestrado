import cv2
import sys
import numpy as np

def function_sift(img1, img2):
	#descritor sift
	sift = cv2.xfeatures2d.SIFT_create()

	#keypoints e descritores
	kp1, des1 = sift.detectAndCompute(img1, None)
	kp2, des2 = sift.detectAndCompute(img2, None)

	return kp1, kp2, des1, des2

def function_surf(img1, img2):
	#descritor surf
	surf = cv2.xfeatures2d.SURF_create()

	#keypoints e descritores
	kp1, des1 = surf.detectAndCompute(img1, None)
	kp2, des2 = surf.detectAndCompute(img2, None)

	return kp1, kp2, des1, des2

def function_orb(img1, img2):
	#descritor orb
	orb = cv2.ORB_create()

	#keypoints e descritores
	kp1, des1 = orb.detectAndCompute(img1, None)
	kp2, des2 = orb.detectAndCompute(img2, None)

	return kp1, kp2, des1, des2

#diretorio da imagem 1
dir_imagem1 = sys.argv[1]

#diretorio da imagem 2
dir_imagem2 = sys.argv[2]

#descritor escolhido
descritor = sys.argv[3]

#prefixo das imagens de saida
saida = sys.argv[4]

#abrindo as imagens com cores
img1color = cv2.imread(dir_imagem1)
img2color = cv2.imread(dir_imagem2)

#abrindo as imagens em tons de cinza
img1 = cv2.imread(dir_imagem1, 0)
img2 = cv2.imread(dir_imagem2, 0)

foi_possivel = True

#inicializacoes para receber os keypoints, descritores
kp1, kp2, des1, des2 = 0, 0, 0, 0

#verifica o descritor escolhido
if descritor == 'sift':
	kp1, kp2, des1, des2 = function_sift(img1, img2)
elif descritor == 'surf':
	kp1, kp2, des1, des2 = function_surf(img1, img2)
elif descritor == 'orb':
	kp1, kp2, des1, des2 = function_orb(img1, img2)
else:
	print('Nao foi possivel aplicar o descritor')
	foi_possivel = False

if foi_possivel:

	#seleciona os melhores matchings
	index_params = dict(algorithm = 1, trees = 5)
	search_params = dict(checks = 50)
	flann = cv2.FlannBasedMatcher(index_params, search_params)
	row_matches = flann.knnMatch(des1,des2,k=2)
	
	matches = []
	for m,n in row_matches:
	    if m.distance < 0.7*n.distance:
	        matches.append(m)

	pts1 = np.float32([kp1[m.queryIdx].pt for m in matches]).reshape(-1,1,2)
	pts2 = np.float32([kp2[m.trainIdx].pt for m in matches]).reshape(-1,1,2)

	#matriz de homografia
	h, status = cv2.findHomography(pts1, pts2, cv2.RANSAC)
	
	#alinhamento das imagens e criacao da imagem panoramica
	result = cv2.warpPerspective(img1color, h, (img2color.shape[1] + img1color.shape[1],img2color.shape[0]))
	result[0:img2color.shape[0], 0:img2color.shape[1]] = img2color

	#desenho entre os pontos correspondentes
	img = cv2.drawMatches(img1color, kp1, img2color, kp2, matches[:20], None, flags=2)

	#salva as imagens
	cv2.imwrite('saida/' + saida + '-panoramica.jpg', result)
	cv2.imwrite('saida/' + saida + '-retas.jpg', img)