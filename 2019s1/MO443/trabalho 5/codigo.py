import numpy as np
import cv2
from sklearn.cluster import KMeans
from sklearn.utils import shuffle
import sys

#diretorio da imagem
dir_imagem = sys.argv[1]

#metodo
metodo = sys.argv[2]

#numero de cores
qtde_cores = int(sys.argv[3])

#nome da imagem de saida
saida = sys.argv[4]

#leitura da imagem
img = cv2.imread(dir_imagem)

#transformar a imagem de 3D para 2D
w, h, d = img.shape
assert d == 3
image_array = np.reshape(img, (w * h, d))

#clusterizacao com kmeans
if metodo == 'k-means++' or metodo == 'random':

	kmeans = KMeans(n_clusters = qtde_cores, init = metodo).fit(image_array)

	#predict os rotulos da imagem
	labels = kmeans.predict(image_array)

	#salvando em um dicionario os centros
	codebook = kmeans.cluster_centers_

	#reconstruindo a imagem
	d = codebook.shape[1]
	img_saida = np.zeros((w, h, d))
	label_idx = 0
	for i in range(w):
		for j in range(h):
			img_saida[i][j] = codebook[labels[label_idx]]
			label_idx += 1

	#salvando a imagem
	cv2.imwrite('saida/' + saida, img_saida)

else:
	print('Nao foi possivel aplicar a tecnica')