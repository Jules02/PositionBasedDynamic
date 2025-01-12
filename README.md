Introduction au C/C++
### Groupe: Emna KILANI & Jules DUPONT
# Projet *Position-Based Dynamics*
Ce projet implémente une simulation de type [Position-Based Dynamics](https://matthias-research.github.io/pages/publications/posBasedDyn.pdf) en dimension 2 avec C++.

### Fonctionnalités implémentées:
- visualisation du contexte de la simulation physique.
- ajout de colliders (classe abstraite Collider), qui permettent de représenter un sol ou un mur (PlanCollider), ou encore un disque impénétrable (SphereCollider).
- créations d'objets (classe Object), qui représentent un agrégat de particules (class Particle).
- application de champs de forces extérieures (par exemple, la gravité) aux objets de la simulation (plus précisement, aux particules qui composent les objets de la simulation).
- activation ou désactivation de la gravité au moyen d'un bouton.
- génération et résolution de différentes contraintes (classe abstraite Constraint), représentant le contact entre un objet et un collider (StaticConstraint), la répulsion entre deux particules appartenant à des objets différents (DynamicConstraint), ou encore l'interaction entre deux particules d'un même objet (LinkConstraint).
- visualisation de l'activation de certaines de ces contraintes, pour le debuggage.
