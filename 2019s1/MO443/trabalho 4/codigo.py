import cv2

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
img2 = cv2.imread(dir_imagem2, 0)

#descritores
sift = cv2.xfeatures2d.SIFT_create()
surf = cv2.xfeatures2d.SURF_create()
orb = cv2.ORB_create()

#keypoints e descritores das imagens
kp1, des1 = sift.detectAndCompute(img1, None)
kp2, des2 = sift.detectAndCompute(img2, None)

#selecionar os melhores matchings
bf = cv2.BFMatcher()
matches = bf.match(des1, des2)
matches = sorted(matches, key = lambda x:x.distance)

img3 = cv2.drawMatches(img1, kp1, img2, kp2, matches[:10], None, flags=2)

plt.imshow(img3)
plt.show()
#img = cv2.drawKeypoints(gray, kp, img1)

#cv2.imshow('imagem', img)
#cv2.waitKey(0)