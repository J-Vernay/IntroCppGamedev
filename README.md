# IntroCppGamedev

Base de code pour le cours d'introduction au C++ pour le développement de jeux vidéo.

Cours par Julien Vernay. Pour me contacter : `edu@jvernay.fr`

## Mise en place

1. Ouvrir Visual Studio 2022
2. Cliquer sur "Cloner un dépôt"
  - Emplacement du dépôt : `https://git.sr.ht/~jvernay/IntroCppGamedev`
  - Chemin : Parcourir -> Bureau -> Nouveau dossier - > `IntroCppGamedev`
3. Appuyer sur "Cloner"
4. Dans la fenêtre de droite "Modifications Git - IntroCppGamedev" :
  - Cliquer sur "Master"
  - Cliquer sur "Nouvelle branche"
	- Spécifier "NOM_Prenom" comme nom de branche
	- Vérifier que "Checkout la branche" est cochée
  - Cliquer sur "Create"
5. Dans la barre de menu en haut :
  - Choisir "Affichage" puis "Terminal"
6. Dans la fenêtre apparue en bas :
  - Taper la commande `tools/premake5 vs2022` puis la touche "Entrée".
7. Fermer Visual Studio 2022
8. Dans le dossier `IntroCppGamedev` sur le bureau:
  - Cliquer sur le dossier `build`
  - Double-cliquer sur `IntroCppGamedev.sln`
9. Dans la fenêtre "Explorateur de solutions" à droite :
  - Clic-droit sur "Exo1"
  - Cliquer sur "Définir en tant que projet de démarrage"
10. Dans la barre d'outils en haut :
  - Cliquer sur la flèche verte "Débogueur Windows local"

Vous devriez obtenir cette fenêtre :

![docs/cours/exo1_image.png](docs/cours/exo1_image.png)

Une fois ceci vérifié, c'est bon, l'environnement de développement est opérationnel ! 😎

11. Dans la fenêtre "Explorateur de solutions" à droite :
  - Déplier `Documentation`
  - Déplier `docs`
  - Déplier `cours`

Vous aurez alors accès à la documentation du cours.
Notamment, les fichiers `exoN.md` correspondent aux sujets de TDs à faire en cours.
Ces fichiers contiennent des questions auxquelles il faut répondre en modifiant directement le fichier.
Commencez en ouvrant `exo1.md`.
