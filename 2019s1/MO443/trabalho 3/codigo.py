import cv2
import numpy as np
import sys

def funcao(imagem):

	#definicao dos kernels
	kernel1 = np.ones((10, 1), np.uint8)
	kernel2 = np.ones((1, 10), np.uint8)
	kernel3 = np.ones((3, 1), np.uint8)

	#dilatacao seguida de erosao, conforme os passos (1) e (2)
	img1 = cv2.morphologyEx(img, cv2.MORPH_CLOSE, kernel1)

	#dilatacao seguida de erosao, conforme os passos (3) e (4)
	img2 = cv2.morphologyEx(img, cv2.MORPH_CLOSE, kernel2)

	#passo(5) - operador logico AND entre as duas imagens dos passos (2) e (4) 
	img = np.logical_and(img1, img2)
	img = np.array(img, dtype=np.uint8)

	#passo(6) - fechamento da imagem do passo (5)
	img = cv2.morphologyEx(img, cv2.MORPH_CLOSE, kernel3)

	return img

#diretorio da imagem
dir_imagem = sys.argv[1]

#como a imagem deve ser salva
saida = sys.argv[2]

#leitura da imagem em tons de cinza
img = cv2.imread('imagens/bitmap.pbm', 0)

img = funcao(img)


cv2.imwrite('saida/' + saida, img)