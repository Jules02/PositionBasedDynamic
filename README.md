Introduction au C/C++
### Groupe: Emna KILANI & Jules DUPONT
# Projet *Position-Based Dynamics*
Ce projet implémente une simulation de type [Position-Based Dynamics](https://matthias-research.github.io/pages/publications/posBasedDyn.pdf) en dimension 2 avec C++.

### Fonctionnalités implémentées:
- visualisation du contexte de la simulation physique.
- ajout de colliders (classe abstraite `Collider`), qui permettent de représenter un sol ou un mur (`PlanCollider`), ou encore un disque impénétrable (`SphereCollider`).
- ajout de ces colliders directement depuis l'interface utilisateur:
  - _ajout d'un_ `PlanCollider`: deux clicks sucessifs permettent de définir un plan via un vecteur directeur. La normale associée est obtenue par une rotation à 90 degrés.
  - _ajout d'un_ `SphereCollider`: un premier click définit le centre du disque, le second fixe son rayon. 
- créations d'objets (classe `Object`), qui représentent un agrégat de particules (class `Particle`).
- application de champs de forces extérieures (par exemple, la gravité) aux objets de la simulation (plus précisement, aux particules qui composent les objets de la simulation).
- activation ou désactivation de la gravité au moyen d'un bouton.
- génération et résolution de différentes contraintes (classe abstraite `Constraint`), représentant le contact entre un objet et un collider (`StaticConstraint`), la répulsion entre deux particules appartenant à des objets différents (`DynamicConstraint`), ou encore l'interaction entre deux particules d'un même objet (`LinkConstraint`).
- visualisation de l'activation de certaines de ces contraintes, pour le debuggage.
- tests basiques.

### Fonctionnalités en cours d'implémentation/à implémenter
- simulation de fluides. On a commencé à implémenter la classe `FluidConstraint`, dont le but est de gérer l'interaction entre deux particules dans le cadre d'un fluide. Plusieurs briques présentées dans la référence _Position Based Fluids_ (Macklin & Müller, 2013) sont déjà implémentées, mais les résultats ne sont pas encore satisfaisants. En particulier, l'étape la plus urgente est d'implémenter une "grille" permettant de rechercher les voisins d'une particule de manière bien plus performante. Avec l'algorithme naïf implémenté, on peut difficilement dépasser les 10 particules dans la simulation sans faire exploser les temps de latence.

### Comment lancer les tests ?
Depuis QtCreator, il faut Run l'exécutable `PositionBasedDynamics_Tests`. Les résultats apparaissent dans l'onglet `Application Output`.

### Bugs majeurs connus
- Bien qu'il semble que l'on a bien implémenté les équations de la ressource equations.pdf associée au sujet, la résolution des contraintes de contact avec les colliders n'est pas exacte. Par exemple, depuis la situation initiale, les objets ne roulent pas exactement sur le collider planaire de gauche mais avec un certain décalage. Le problème est peut-être dû à la façon dont on affiche les colliders.

### Références
- Müller et al., 2006. _Position Based Dynamics_
- Macklin & Müller, 2013. _Position Based Fluids_
