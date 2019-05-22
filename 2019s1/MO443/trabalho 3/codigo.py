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
	linhas_cont = 0

	#retorna o que eh texto
	linhas = np.array([[]])

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
				linhas = cv2.rectangle(img_original, (x, y), (x + a, y + b), 1, 5)
				linhas_cont += 1

		cont += 1

	print('\nForam encontradas ' + str(linhas_cont) + ' linhas\n')

	return linhas

def encontra_palavras(img):

	#kernel para encontrar palavras
	kernel1 = np.ones((1, 10), np.uint8)
	kernel2 = np.ones((100, 1), np.uint8)

	#transformar a imagem em fundo preto e texto em branco
	imgaux = cv2.bitwise_not(img)

	#faz o fechamento da imagem
	img1 = cv2.morphologyEx(imgaux, cv2.MORPH_CLOSE, kernel1)

	#faz o fechamento da imagem
	img2 = cv2.morphologyEx(imgaux, cv2.MORPH_CLOSE, kernel2)

	#operacao AND entre img1 e img2
	img1 = cv2.bitwise_and(img1, img2)

	#fechamento da imagem
	img1 = cv2.morphologyEx(img1, cv2.MORPH_CLOSE, kernel1)

	#encontra os componentes conexos
	nlabels, labels, stats, centroids = cv2.connectedComponentsWithStats(img1)

	imgsaida = np.array([])

	#contador de componentes conexos, comeca com -1 porque o componente 0 eh a imagem inteira
	cont = -1

	#contador de palavras
	palavras = 0

	for components in stats:
			
		x = components[0]
		y = components[1]
		a = components[2]
		b = components[3]

		if cont != -1:

			#contador de pixels pretos
			pretos = 0

			#contador de transicoes de preto para branco
			transicoes = 0

			#tamanho do componente
			tam = ((x + a) - x) * ((y + b) - y)

			for i in range(y, y+b):
				for j in range(x, x+a):
					if img[i,j] == 0:
						pretos += 1
						if img[i-1,j] != 0 or img[i+1,j] != 0 or img[i,j-1] != 0 or img[i,j+1] != 0: 
							transicoes += 1

			if pretos > 0 and (pretos/tam) > 0.15 and (transicoes/(pretos+1)) < 0.6 and (pretos/tam) < 0.6:
				imgsaida = cv2.rectangle(img, (x, y), (x+a, y+b), 1, 5)
				palavras += 1

		cont += 1

	print('Foram encontradas ' + str(palavras) + ' palavras\n')

	return imgsaida



def transforma_pbm(img):
	
	img[img < 128] = 0
	img[img >= 128] = 1 

	return img

#diretorio da imagem
dir_imagem = sys.argv[1]

#como a imagem deve ser salva
saida = sys.argv[2]

#leitura da imagem em tons de cinza
img = cv2.imread(dir_imagem, 0)

#aplicacao dos passos (1) - (6)
imgsaida = operadores_morfologicos(img)

#aplicacao dos passos (7) - (9)
linhas = encontra_texto(imgsaida, img)

#tive que fazer novamente a leitura porque por algum motivo a imagem era perdida
img = cv2.imread(dir_imagem, 0)

#encontra as palavras da imagem
palavras = encontra_palavras(img)

#funcao para transformar no formato pbm
linhas = transforma_pbm(linhas)
palavras = transforma_pbm(palavras)

cv2.imwrite('saida/' + saida + '-linhas.pbm', linhas)
cv2.imwrite('saida/' + saida + '-palavras.pbm', palavras)