# Projet_Mif23_Analyse

##Dépendances et librairies

* Cmake : Cmake est requis pour pouvoir compiler le projet
	Téléchargez la version qui correspond à votre Système d'exploitation ici : https://cmake.org/download/
	* Installation sous Linux/Mac
		1. Ouvrez un terminal
		2. Déplacez vous dans le dossier téléchargé
		3. lancez les commandes
			* ./bootstrap
			* make
			* make install

* OpenCV : opencv est requis pour pouvoir compiler et lancer le projet
	* Installation sous Linux
		1. Dans un terminal, lancez la commande 
			sudo apt-get install libcv-dev libcvaux-dev libhighgui-dev

## Compilation
L'archive doit contenir ce README, un dossier src, un dossier Data et un fichier CMakeLists.txt
Pour compiler le programme :
	* Extrayez l'archive
	* Dans un terminal, déplacez vous dans le dossier contenant les éléments extraits de l'archive
	* lancez les commandes :
		1. cmake .
		2. make clean
		3. make

## Éxécution
Pour lancer le programme :
	* Lancer la commande :
		* ./Analyse Data/BG-camera-01.avi Data/ACQ-camera-01.avi 
	* Pour quittez le programme en cours d'éxécution, appuyez sur 'q'


