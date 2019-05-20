import cv2
import numpy as np
import sys


def operadores_morfologicos(img):

	#definicao dos kernels
	kernel1 = np.ones((1, 100), np.uint8)
	kernel2 = np.ones((200, 1), np.uint8)
	kernel3 = np.ones((1, 30), np.uint8)

	#transformar a imagem em fundo preto e texto em branco
	img = cv2.bitwise_not(img)

	#dilatacao seguida de erosao, conforme os passos (1) e (2)
	img1 = cv2.morphologyEx(img, cv2.MORPH_CLOSE, kernel1)

	#dilatacao seguida de erosao, conforme os passos (3) e (4)
	img2 = cv2.morphologyEx(img, cv2.MORPH_CLOSE, kernel2)

	#passo(5) - operador logico AND entre as duas imagens dos passos (2) e (4) 
	img = cv2.bitwise_and(img1, img2)

	#passo(6) - fechamento da imagem do passo (5)
	img = cv2.morphologyEx(img, cv2.MORPH_CLOSE, kernel3)

	return img

def componentes_conexos(img, img_original):
	nlabels, labels, stats, centroids = cv2.connectedComponentsWithStats(img)

	cont = 0

	imgfinal = np.array([[]])

	for components in stats:
		x = components[0]
		y = components[1]
		a = components[2]
		b = components[3]

		pretos = 0
		tam = ((x + a) - x) * ((y + b) - y) 

		imgfinal = cv2.rectangle(img_original, (x, y), (x + a, y + b), 1, 5)

		cont += 1

	cv2.imwrite('cc.png', imgfinal)

	print('Foram encontrados ' + str(cont) + ' componentes conexos')

	return imgfinal

#diretorio da imagem
dir_imagem = sys.argv[1]

#como a imagem deve ser salva
saida = sys.argv[2]

#leitura da imagem em tons de cinza
img = cv2.imread('imagens/bitmap.pbm', 0)

#aplicacao dos passos (1) - (6)
imgsaida = operadores_morfologicos(img)

#aplicacao dos passos (7) - (9)
imgfinal = componentes_conexos(imgsaida, img)
	
cv2.imwrite('saida/' + saida, imgfinal)