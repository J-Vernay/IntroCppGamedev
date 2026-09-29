# Références UV des images sur le projet DinoRanch

## Dinosaures

Les images de dinosaures sont dans la spritesheet `dinosaurs.bmp`,
disposées sur une grille de **24x24 pixels**.

![Texture des dinosaures](dinosaurs.bmp)

Les animations consistent à enchaîner en boucle différents sprites.
Dans la spritesheet, ces sprites correspondent à des positions U :

- **U=(0, 24, 48, 72)** pour l'animation quand le dinosaure reste sur place. (8 images par seconde)
- **U=(96, 120, 144, 168, 192, 216)** pour l'animation de marche. (8 images par seconde)
- **U=(336, 360, 384)** pour l'animation de dégâts. (8 images par seconde)
- **U=(432, 456, 480, 504, 528, 552)** pour l'animation de course. (16 images par seconde)

Les dinosaures de différentes couleurs correspondent à des positions V :

- **V=0** pour le dinosaure bleu
- **V=24** pour le dinosaure rouge
- **V=48** pour le dinosaure jaune
- **V=72** pour le dinosaure vert

## Terrain

Les images du terrain sont dans le tileset `terrain.bmp`.

![Texture du terrain](terrain.bmp)

Les tuiles du tileset font une taille de **16x16 pixels**.
Le terrain de **256x192 pixels** correspond donc à **16x12 tuiles**.
Les arbres font une taille de **48x72 pixels**.

La spritesheet est composée de :

- **UV = (0,0)** pour la couleur de l'océan
- **UV = (16,0)** pour la couleur du terrain
- **UV = (0,16)** pour la tuile du coin en haut à gauche
- **UV = (32,16)** pour la tuile du coin en haut à droite
- **UV = (0,48)** pour la tuile du coin en bas à gauche
- **UV = (32,48)** pour la tuile du coin en bas à droite
- **UV = (16,16)** pour la tuile à répéter sur la frontière en haut
- **UV = (0,32)** pour la tuile à répéter sur la frontière à gauche
- **UV = (32,32)** pour la tuile à répéter sur la frontière à droite
- **UV = (16,48)** pour la tuile à répéter sur la frontière en bas
- **UV = (32,0)** pour la fleur 1
- **UV = (48,0)** pour la fleur 2
- **UV = (64,0)** pour la fleur 3
- **UV = (de 48,16 à 80,64)** pour l'arbre 1
- **UV = (de 48,64 à 80,112)** pour l'arbre 2
- **UV = (de 48,112 à 80,160)** pour l'arbre 3
- **UV = (de 48,160 à 80,208)** pour l'arbre 4

Pour obtenir les animations des tuiles de terrain, il faut ajouter dans l'ordre **V += (0, 48, 96, 144)**.

Pour obtenir les saisons, il faut ajouter dans l'ordre **U += (0, 80, 160, 240)**.

## Animaux

Les images d'animaux sont dans la spritesheet `animals.bmp`.

![Texture des animaux](animals.bmp)

Chaque sprite fait une taille de **32x32 pixels**.

Les animations consistent à enchaîner en boucle 4 sprites,
suivant les positions **U=(0, 32, 64, 96)**.

Les différentes animations correspondent à des positions V :

- **V = 0** pour marcher sur le côté (à mettre en miroir pour aller vers la droite)
- **V = 32** pour marcher vers le bas
- **V = 64** pour marcher vers le haut

Les différents animaux correspondent à des décalage de positions U :

- **U += 0 ou 128** pour les deux types de cochon.
- **U += 256 ou 384** pour les deux types de vache.
- **U += 512 ou 640** pour les deux types de mouton.
- **U += 768 ou 896** pour les deux types d'autruche.

