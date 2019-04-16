import cv2
import numpy as np 
import sys

#aplica o filtro
def filt(imagem, filtro):
	return cv2.filter2D(imagem, -1, filtro)

#se o filtro for sqrt(h3^2 + h4^2), aplica o filtro h3 e h4 e depois realiza a conta
def filth3h4(imagem):
	imagemh3 = filt(imagem, h3)
	imagemh4 = filt(imagem, h4)
	return np.hypot(imagemh3, imagemh4).astype('int8')

def fourier(img):
	sigma = float(input('Digite o valor de sigma: '))
	tam = int(input('Digite o tamanho do filtro: '))

	#transformada de fourier
	dft = np.fft.fft2(img)

	#transforma frequencia no centro
	dft_shift = np.fft.fftshift(dft)

	#pegando os valores de linha e coluna da imagem
	row, col = img.shape[0], img.shape[1]
	rrow, ccol = img.shape[0]//2, img.shape[1]//2

	#construcao do filtro gaussiano
	mask = np.zeros((row, col))
	kernel = cv2.getGaussianKernel(tam, sigma)
	kernel = kernel*kernel.T

	#pegando os valores de linha e coluna do filtro
	krow, kcol = kernel.shape[0]//2, kernel.shape[1]//2

	#padding com 0 no filtro gaussiano
	mask[rrow-krow:rrow+krow, ccol-kcol:ccol+kcol] = 1 - kernel

	#convolucao
	fshift = dft_shift * mask
	
	#retornando para dominio espacial
	f_ishift = np.fft.ifftshift(fshift)
	img_back = np.fft.ifft2(f_ishift)
	
	return np.abs(img_back)
	


#filtros
h1 = np.array([[0, 0, -1, 0, 0], [0, -1, -2, -1, 0], [-1, -2, 16, -2, -1], [0, -1, -2, -1, 0], [0, 0, -1, 0, 0]], np.float32)
h2 = np.array([[1, 4, 6, 4, 1], [4, 16, 24, 16, 4], [6, 24, 36, 24, 6], [4, 16, 24, 16, 4], [1, 4, 6, 4, 1]], np.float32) / 256
h3 = np.array([[-1, 0, 1], [-2, 0, 2], [-1, 0, 1]], np.float32)
h4 = np.array([[-1, -2, -1], [0, 0, 0], [1, 2, 1]], np.float32)

#diretorio da imagem
dirimagem =  sys.argv[1]

#filtro escolhido
filtro = sys.argv[2]

#saida da imagem
saida = sys.argv[3]

#transformar a imagem em escala de cinza
imagem = cv2.imread(dirimagem, 0)

#verifica qual foi o filtro escolhido
if filtro == 'h1':
	imagem = filt(imagem, h1)
elif filtro == 'h2':
	imagem = filt(imagem, h2)
elif filtro == 'h3':
	imagem = filt(imagem, h3)
elif filtro == 'h4':
	imagem = filt(imagem, h4)
elif filtro == 'h3+h4':
	imagem = filth3h4(imagem)
elif filtro == 'gauss':
	imagem = fourier(imagem)
else:
	print("nao foi possivel aplicar o filtro escolhido")

#salva a imagem no diretorio de saida
cv2.imwrite('saida/' + saida, imagem)