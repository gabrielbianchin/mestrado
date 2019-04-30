import cv2
import numpy as np 
import sys

def direita (imagem, altura, largura):
	for i in range(1, altura-1):
		for j in range(1, largura-1):
			erro = 0
			if imagem[i][j] > 128:
				erro = imagem[i][j] - 255
				imagem[i][j] = 255
			else:
				erro = imagem[i][j] - 0
				imagem[i][j] = 0

			imagem[i][j+1] = imagem[i][j+1] + (erro * (7/16))
			imagem[i+1][j-1] = imagem[i+1][j-1] + (erro * (3/16))
			imagem[i+1][j] = imagem[i+1][j] + (erro * (5/16))
			imagem[i+1][j+1] = imagem[i+1][j+1] + (erro * (1/16))

	return imagem

def esquerda(imagem, altura, largura):
	for i in range(1, altura-1):
		for j in range(largura-2, 1, -1):
			erro = 0
			if imagem[i][j] > 128:
				erro = imagem[i][j] - 255
				imagem[i][j] = 255
			else:
				erro = imagem[i][j] - 0
				imagem[i][j] = 0

			imagem[i][j-1] = imagem[i][j-1] + (erro * (7/16))
			imagem[i+1][j+1] = imagem[i+1][j-1] + (erro * (3/16))
			imagem[i+1][j] = imagem[i+1][j] + (erro * (5/16))
			imagem[i+1][j-1] = imagem[i+1][j-1] + (erro * (1/16))

	return imagem

def esqdir(imagem, altura, largura):
	for i in range(1, altura-1):
		#verifica se deve varrer a imagem da esquerda para direita ou da direita para a esquerda, iniciando da esquerda para direita
		if i%2 != 0:
			#varre a imagem da esquerda para direita
			for j in range(1, largura-1):
				erro = 0
				if imagem[i][j] > 128:
					erro = imagem[i][j] - 255
					imagem[i][j] = 255
				else:
					erro = imagem[i][j] - 0
					imagem[i][j] = 0

				imagem[i][j+1] = imagem[i][j+1] + (erro * (7/16))
				imagem[i+1][j-1] = imagem[i+1][j-1] + (erro * (3/16))
				imagem[i+1][j] = imagem[i+1][j] + (erro * (5/16))
				imagem[i+1][j+1] = imagem[i+1][j+1] + (erro * (1/16))
		else:
			#varre a imagem da direita para esquerda
			for j in range(largura-2, 1, -1):
				erro = 0
				
				if imagem[i][j] > 128:
					erro = imagem[i][j] - 255
					imagem[i][j] = 255
				else:
					erro = imagem[i][j] - 0
					imagem[i][j] = 0

				imagem[i][j-1] = imagem[i][j-1] + (erro * (7/16))
				imagem[i+1][j+1] = imagem[i+1][j-1] + (erro * (3/16))
				imagem[i+1][j] = imagem[i+1][j] + (erro * (5/16))
				imagem[i+1][j-1] = imagem[i+1][j-1] + (erro * (1/16))
	
	return imagem

def diresq(imagem, altura, largura):
	for i in range(1, altura-1):
		#verifica se deve varrer a imagem da esquerda para direita ou da direita para a esquerda, iniciando da direita para esquerda
		if i%2 == 0:
			#varre a imagem da esquerda para direita
			for j in range(1, largura-1):
				erro = 0
				if imagem[i][j] > 128:
					erro = imagem[i][j] - 255
					imagem[i][j] = 255
				else:
					erro = imagem[i][j] - 0
					imagem[i][j] = 0

				imagem[i][j+1] = imagem[i][j+1] + (erro * (7/16))
				imagem[i+1][j-1] = imagem[i+1][j-1] + (erro * (3/16))
				imagem[i+1][j] = imagem[i+1][j] + (erro * (5/16))
				imagem[i+1][j+1] = imagem[i+1][j+1] + (erro * (1/16))
		else:
			#varre a imagem da direita para esquerda
			for j in range(largura-2, 1, -1):
				erro = 0
				
				if imagem[i][j] > 128:
					erro = imagem[i][j] - 255
					imagem[i][j] = 255
				else:
					erro = imagem[i][j] - 0
					imagem[i][j] = 0

				imagem[i][j-1] = imagem[i][j-1] + (erro * (7/16))
				imagem[i+1][j+1] = imagem[i+1][j-1] + (erro * (3/16))
				imagem[i+1][j] = imagem[i+1][j] + (erro * (5/16))
				imagem[i+1][j-1] = imagem[i+1][j-1] + (erro * (1/16))
	
	return imagem

#diretorio da imagem
dirimagem = sys.argv[1]

#metodo escolhido
metodo = sys.argv[2]

#como a imagem deve ser salva
saida = sys.argv[3]

#abrir imagem em tons de cinza
imagem = cv2.imread(dirimagem, 0)

#pegando altura e largura da imagem
altura, largura = imagem.shape[0], imagem.shape[1]

if metodo == 'fs':
	print('\n************************************************************')
	print('Escolha um dos metodos a seguir:')
	print('1 - Varredura da esquerda para direita.')
	print('2 - Varredura da direita para esquerda.')
	print('3 - Varredura alternada iniciando da esquerda para direita.')
	print('4 - Varredura alternada iniciando da direita para esquerda.')
	print('************************************************************\n')
	opcao = int(input('Selecione uma das opcoes anteriores: '))
	if opcao == 1:
		imagem = direita(imagem, altura, largura)
	elif opcao == 2:
		imagem = esquerda(imagem, altura, largura)
	elif opcao == 3:
		imagem = esqdir(imagem, altura, largura)
	elif opcao == 4:
		imagem = diresq(imagem, altura, largura)
	else:
		print('Opcao invalida.\n')

print(imagem / 25.5)