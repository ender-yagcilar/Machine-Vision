# EE576 Pr5
# Generating a subset of images that contain a selected category

from pycocotools.coco import COCO
import numpy as np
import skimage.io as io
import matplotlib.pyplot as plt
import os,shutil

dataDir='..'
dataType='val2017'
annFile='{}/annotations/instances_{}.json'.format(dataDir,dataType)

# initialize COCO api for instance annotations
coco=COCO(annFile)

# display COCO categories and supercategories
cats = coco.loadCats(coco.getCatIds())
nms=[cat['name'] for cat in cats]
print('COCO categories: \n{}\n'.format(' '.join(nms)))

nms = set([cat['supercategory'] for cat in cats])
print('COCO supercategories: \n{}'.format(' '.join(nms)))

# get all images containing the given category
catIds = coco.getCatIds(catNms=['truck'])   # Truck ve Bus kullanıldı
imgIds = coco.getImgIds(catIds=catIds )

# print the number of images containing the given category
print('Number of images containing the given category: ' + str(len(imgIds)))

# For categories like person, the number of images coule be large. You can set a max number of images here.
max_subset_size = 500

# path to destination directory for the subset
dest_dir = '/generated_subset'
cwd = os.getcwd()

# remove the subset folder from any previous attempt, and make a new one
shutil.rmtree(cwd+dest_dir)
os.makedirs(cwd+dest_dir)

# Fill the subset folder with the images containing the given category
for img_num in range(0,min(max_subset_size,len(imgIds))):
    img = coco.loadImgs(imgIds)[img_num]
    anns_ids = coco.getAnnIds(imgIds=img['id'], catIds=catIds, iscrowd=None)
    anns = coco.loadAnns(anns_ids)
    mask = 255*coco.annToMask(anns[0])
    #plt.imshow(mask)
    #plt.show()
   # I = io.imread(img['coco_url'])
    io.imsave(cwd+dest_dir+'/'+img['file_name'],mask)

