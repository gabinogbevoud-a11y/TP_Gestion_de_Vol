# Application de gestion d’une agence de voyage

Projet C++ orienté objet qui permet à une agence de gérer les passagers, les vols, les réservations, les paiements et les annulations depuis un menu console.

## Contenu du dépôt

| Fichier | Rôle |
| --- | --- |
| `TP_Gestion_Vol.pdf` | Analyse complète du sujet et réponses aux consignes de modélisation et d’implémentation. |
| `TP_Gestion_Vol_Diag_Clas.puml` | Diagramme UML de classes en PlantUML. |
| `TP_Gestion_Vol_Diag_Clas.png` | Export du diagramme PlantUML en image pour une consultation rapide. |
| `TP_Gestion_Imple_C.cpp` | Code source de l’application console C++. |
| `README.md` | Présentation du projet, instructions de compilation et règles principales. |

## Fonctionnalités

- Enregistrer un passager et ajouter un vol.
- Afficher les vols disponibles et rechercher un trajet.
- Créer une réservation et consulter les réservations.
- Enregistrer un paiement ou annuler une réservation.
- Afficher les informations d’une réservation et quitter le programme.

## Règles de gestion

1. Un passager doit être enregistré avant de réserver.
2. Le nombre de réservations actives ne dépasse pas la capacité du vol.
3. Un passager ne peut réserver qu’une seule fois un même vol, même si sa réservation précédente a été annulée.
4. Chaque réservation concerne un seul passager et un seul vol.
5. La réservation est confirmée après validation des identifiants, de l’absence de doublon et de la disponibilité.
6. Une réservation annulée ne peut pas être payée.
7. Si une réservation payée est annulée, son paiement passe à l’état `REFUNDED`.

Le paiement est simulé par une confirmation dans l’application; aucun paiement bancaire réel n’est effectué. Les données sont conservées en mémoire et disparaissent à la fermeture du programme.

## Compilation et lancement

Il faut disposer d’un compilateur C++ compatible avec C++17, par exemple `g++`.

```bash
g++ -std=c++17 -Wall -Wextra -pedantic TP_Gestion_Imple_C.cpp -o TP_Gestion_Imple_C
./TP_Gestion_Imple_C
```

Sous Windows, lancer `TP_Gestion_Imple_C.exe` après la compilation.

## Générer l’image PNG du diagramme

Avec Java installé, lancer cette commande depuis le dossier du dépôt. Le fichier `plantuml.jar` doit se trouver dans ce dossier; sinon, remplacer son nom dans la commande par le chemin vers le JAR PlantUML :

```bash
java -jar plantuml.jar -tpng TP_Gestion_Vol_Diag_Clas.puml
```

PlantUML crée automatiquement `TP_Gestion_Vol_Diag_Clas.png` à côté du fichier source `TP_Gestion_Vol_Diag_Clas.puml`. L’image PNG doit être mise à jour après toute modification du diagramme.

## Modèle de classes

- `Passenger` contient les informations d’un passager.
- `Flight` décrit le trajet, le départ, la capacité et le prix d’un vol.
- `Reservation` associe un passager et un vol, puis suit l’état de la réservation et du paiement.
- `Agency` gère les collections et applique les contrôles métier.

Une réservation relie exactement un passager et un vol. Un passager et un vol peuvent apparaître dans plusieurs réservations, sous réserve des règles de doublon et de capacité.

## Parcours de démonstration

1. Enregistrer un passager.
2. Ajouter un vol avec une capacité supérieure à zéro.
3. Afficher ou rechercher le vol pour relever son identifiant.
4. Effectuer la réservation avec les identifiants du passager et du vol.
5. Consulter la réservation, enregistrer le paiement ou l’annuler.

Le PDF contient l’analyse détaillée de l’énoncé, les réponses directes aux consignes UML et C++, ainsi qu’une grille de vérification fonctionnelle.
