### Exercice Code Technique

Le fichier `calibration.csv` fourni est le résultat d'une calibration d'un couple boiter photo et objectif permettant de connaitre les réglages à effectuer sur l’appareil photo pour obtenir une image nette aux profondeurs voulues (profondeurs orthogonales à l'appareil).
Les images `depth*.png` fournis sont des images au format PNG 16bit où chaque pixel représente la profondeur (orthogonale en mm) de la scène capturée par l'appareil photo (dans le repère de l'appareil).

Ecrire une application C++ en ligne de commande qui prend les deux fichiers (calibration et image de depth) en paramètre et qui simule le shot des images nécessaires pour avoir l’intégralité de la scène vue nette.
Il faudra écrire une api qui représente le contrôleur de l'appareil photo et qui affiche les actions actions appelées dans la console (set, get...). Le réglage des paramètres de la camera est une action asynchone qui nécessite de vérifier l'état des paramètres avant de prendre une photo. Pour chacune des photos, un masque des zones vues nette est attendu.
Il faudrait également écrire des tests unitaires sur une fonctionnalité au choix.

Vocabulaire 

| FD | Distance du capteur à la cible |
|----|--------------------------------|
| PPN | Premier plan Net |
| DPN | Dernier Plan Net |
| Focus Position | Paramètre moteur du Focus |



### Prerequisite
For time reasons I chose opencv to handle the pixel matrices. With more time I would have likely chose libpng and more standard c++ approach to avoid such a dependency.

```
sudo apt-get install libopencv-dev
```

### Build & Run unit tests

Ensure you have gcc or any c++ compiler added into your system path. Then, run the following commands on Windows for example:

Run directly from project root directory:
```
cmake -S . -B build
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```