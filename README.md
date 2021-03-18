# WomxnDevelopUbisoft

## Description

Le joueur commence la partie dans un labyrinthe, objectif trouver la sortie dans le temps imparti. \
Dans cette forêt enigmatique se cache des ennemis qui n'hesiteront pas a attaquer en vous voyant, si vous perdez la vie, votre esprit survit et s'échappe de votre corp, vous devenez ainsi un fantome qui peut se deplacer dans le labyrinthe. \
Mais la durée de vie de cet esprit est limité, après un certain temps, votre corp et votre esprit fusionnent et la recherche de la sortie peut continuer.

## Précision

*Voir le fichier example.png*

* Niveau :
  * 2D
  * Design : Foret
  * Temps : 5 min

* Joueur( )
  * Peut attaquer, ramasser/utiliser des objets  
  * Le timer continue
* Fantome :
  * Ne peut pas interragir avec les objets ou les ennemis
  * Si le fantome retourne au corp avant la fin du temps imparti, le joueur renaît avec 100% de sa vie sinon 50%
  * Le timer ne s'ecoule plus
* Objets :
  * Potion : redonne 20 point de vie au joueur
  * Armes : Epee ou arme a distance ( couteaux,arc)
* Ennemi
  * Dinosaure : fort, peu rapide, attaque au corp a corp
  * Ninja : fort, rapide, attaque a distance 

## Ajout supplémentaire 

* Obstacle ( porte ) que seul le fantome peut traverser, 