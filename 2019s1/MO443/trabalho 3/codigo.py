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

def encontra_texto(img, img_original):
	
	#encontrar componentes conexos
	nlabels, labels, stats, centroids = cv2.connectedComponentsWithStats(img)

	#contador de componentes conexos, comeca com -1 porque o componente 0 eh a imagem inteira
	cont = -1

	#contador de linhas
	linhas = 0

	#retorna o que eh texto
	texto = np.array([[]])

	#lista utilizada para detectar palavras em cada linha
	posicoes = []

	for components in stats:

		x = components[0]
		y = components[1]
		a = components[2]
		b = components[3]

		#como a figura inteira eh contada como o primeiro componente, removemos esse componente da imagem
		if cont != -1:

			#contador de pixels pretos
			pretos = 0

			#contador de transicoes de preto para branco
			transicoes = 0

			#tamanho do componente
			tam = ((x + a) - x) * ((y + b) - y)

			for i in range(y, y+b):
				for j in range(x, x+a):
					if img_original[i,j] == 0:
						pretos += 1
						if img_original[i-1,j] != 0 or img_original[i+1,j] != 0 or img_original[i,j-1] != 0 or img_original[i,j+1] != 0: 
							transicoes += 1

			if (pretos/tam) < 0.4 and pretos > 0 and (transicoes/pretos) > 0.4:
				texto = cv2.rectangle(img_original, (x, y), (x + a, y + b), 1, 5)
				linhas += 1
				posicoes.append([y, y+b, x, x+a])

		cont += 1

	posicoes = np.array(posicoes)

	print('\nForam encontrados ' + str(cont) + ' componentes conexos')
	print('Foram encontradas ' + str(linhas) + ' linhas de palavras')

	return texto, posicoes

def encontra_palavras(posicoes, img):

	#kernel para encontrar palavras
	kernel1 = np.ones((1, 100), np.uint8)
	kernel2 = np.ones((50, 1), np.uint8)

	for i in range(len(posicoes)):
		x1, x2, y1, y2 = posicoes[i]

		imgaux = cv2.bitwise_not(img[x1:x2,y1:y2])

		img1 = cv2.morphologyEx(imgaux, cv2.MORPH_CLOSE, kernel1)

		img2 = cv2.morphologyEx(imgaux, cv2.MORPH_CLOSE, kernel2)

		img1 = cv2.bitwise_and(img1, img2)

		nlabels, labels, stats, centroids = cv2.connectedComponentsWithStats(img1)

		#contador de palavras
		cont = 0

		imgsaida = np.array([])

		for components in stats:
			
			x = components[0]
			y = components[1]
			a = components[2]
			b = components[3]

			imgsaida = cv2.rectangle(img[x1:x2,y1:y2], (x, y), (x+a, y+b), 1, 5)

		cv2.imshow('imagem', imgsaida)
		cv2.waitKey(0)


def transforma_pbm(img):
	
	img[img < 128] = 0
	img[img >= 128] = 1 

	return img

#diretorio da imagem
dir_imagem = sys.argv[1]

#como a imagem deve ser salva
saida = sys.argv[2]

#leitura da imagem em tons de cinza
img = cv2.imread('imagens/bitmap.pbm', 0)

#aplicacao dos passos (1) - (6)
imgsaida = operadores_morfologicos(img)

#aplicacao dos passos (7) - (9)
texto, posicoes = encontra_texto(imgsaida, img)

palavras = encontra_palavras(posicoes, img)

texto = transforma_pbm(texto)

cv2.imwrite('saida/' + saida, texto)