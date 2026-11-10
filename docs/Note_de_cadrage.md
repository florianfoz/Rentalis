# Note de cadrage – Rentalis

8 novembre 2026 – Auteur : Florian FOZ

Documentation rétrospective, rédigée a posteriori à partir du projet d'origine.

## 1 – Contexte

Rentalis est une application de gestion locative conçue pour un professionnel de l'immobilier (client anonymisé), proche de l'auteur. Après que l'auteur lui a présenté les avantages d'une application métier, le client souhaite remplacer l'ensemble de ses processus par une solution numérique centralisée.

Avant la mise en place de cet outil, le client gérait ses locations et ses loyers sur Excel, sans macros, avec peu de champs calculés, au moyen de tableaux discontinus et sans tableaux croisés dynamiques.

Concernant les factures, elles étaient réalisées de manière irrégulière à l'aide de logiciels bureautiques tels que Word ou Excel. Les données étaient renseignées manuellement à chaque nouvelle facture et pour chaque locataire.

L'envoi des factures s'effectuait principalement par e-mail, rédigé manuellement. Les pièces jointes étaient de formats variables (`.docx`, `.xlsx`, `.pdf`), dont certains ne garantissaient pas l'inaltérabilité des documents.

Les quittances de loyer étaient réalisées sur papier, à l'aide d'un carnet de quittances prévu à cet effet. Leur envoi s'effectuait principalement par e-mail, ce qui nécessitait de numériser au préalable les feuillets détachables du carnet.

Les calculs étaient réalisés en parallèle, à l'aide des tableurs, d'une calculatrice et d'un crayon, le client ayant peu confiance dans les champs calculés de ses tableurs.

## 2 – Problèmes et besoins

Le principal problème du client résidait dans la fragmentation de ses processus entre différents supports numériques et physiques, ainsi que dans l'absence d'interopérabilité entre ces différents supports.

Cette problématique engendrait une succession d'inconvénients :
- Répétition des saisies manuelles.
- Coquilles dans les textes.
- Erreurs lors des reports de données.
- Mauvaise mise en forme des valeurs numériques.
- Doublons.
- Pertes de données et documents égarés.
- Oublis.
- Difficultés à se repérer dans les processus.
- Charge cognitive importante.
- Difficulté à obtenir une vue globale de l'activité et à en mesurer les performances.

## 3 – Objectifs

**1. Unification des processus au sein d'une solution numérique unique, reposant sur une source unique de données.**
- Critère : Saisie unique d'une donnée, accessible dans les autres processus.
- Critère : Absence de ressaisie des informations déjà présentes pour générer les données pertinentes (factures, quittances, e-mails).

**2. Simplification et séparation des différents processus en plusieurs domaines fonctionnels : loyers, locataires, bâtiments, quittances, factures, dommages, réparations, etc.**
- Critère : Réduction du nombre de clics et mise en place d'une action par fonctionnalité.

**3. Préservation et auditabilité des données.**
- Critère : Vue globale et filtrable (par année, bâtiment, historique des locataires, loyers, dommages, entretiens, etc.).
- Critère : Données brutes portables, accessibles et indépendantes du logiciel.

**Critères globaux de réussite :**
- Accélération des processus : moins de clics et moins de changements d'environnement de travail.
- Amélioration de l'efficacité de l'activité.
- Réduction des erreurs humaines.
- Allègement de la charge cognitive.

## 4 – Périmètre

**Fonctionnalités primaires (Must)**
- Implémentation de chaque processus dans des menus séparés.
- Formulaires de création d'enregistrements dans la base de données SQL.
- Tableau de bord avec filtres par date, bâtiment et autres données pertinentes.
- Génération automatique de factures et de quittances.
- Système de sauvegarde, d'importation et d'exportation des données.
- Logiciel local, sans dépendance à Internet.

**Fonctionnalités de confort (Should/Could)**
- Sauvegarde automatique.
- Vérification de l'intégrité des données renseignées.
- E-mails préremplis.
- Formulaires préremplis.
- Génération de prompts à partir de données anonymisées, destinés à être copiés-collés dans un chatbot, sans recours à des API externes invasives.

**Fonctionnalités écartées (Won't)**
- **Envoi automatisé d'e-mails.**
  - Raison : Cette fonctionnalité nécessiterait une connexion à Internet ou l'accès aux API de fournisseurs de messagerie tiers. L'utilisation de `mailto:` résout partiellement le problème tout en préservant la volonté du client de garder le contrôle sur ses processus.
- **Assistant virtuel local de type « Clippy ».**
  - Raison : L'accompagnement du client répond à ce besoin. Un tel assistant alourdirait inutilement le projet.

## 5 – Parties prenantes

**Acteurs**
- Client et utilisateur unique : professionnel de l'immobilier.
- Locataires : destinataires de certains documents et e-mails (quittances, factures).
- Business Analyst et développeur : Florian FOZ.

**Gouvernance**

| Rôle | Acteur |
|-|-|
| Expression des besoins | Client |
| Communication et écoute | Client, Florian FOZ |
| Choix techniques | Florian FOZ |
| Classification des demandes | Florian FOZ, client |
| Ergonomie et fonctionnalités | Client, Florian FOZ |
| Développement, tests, déploiement et maintenance | Florian FOZ |
| Retour d'expérience (feedback) | Client, locataires (sur les factures, quittances et e-mails reçus) |

## 6 – Mode de travail

Développement itératif à la demande, dans une logique agile informelle : le client signale un besoin ou un problème, puis l'auteur le contacte pour lever les éventuels doutes, transmettre des informations et communiquer des captures d'écran sur l'avancement du projet. Des démonstrations sont réalisées lors des rencontres.

Classement des demandes selon la méthode MoSCoW (voir partie 4 – Périmètre).

## 7 – Contraintes

**Période :** 01/05/2025 – 05/12/2025

**Budget :** non communiqué.

**Logiciel :**
- Fonctionnement hors ligne.
- Logiciel léger et réactif.
- Système d'exploitation du client : Windows 11.

**Développement :**
- Un seul développeur (auteur).

**Client :**
- Souhaite conserver ses habitudes tout en optimisant ses processus.
- Disponibilité variable et changements de choix en matière de conception.
- Soucieux de la protection de sa vie privée et de la confidentialité de son activité.
- Souhaite utiliser l'IA sans en dépendre ni exposer excessivement ses informations.

## 8 – Hypothèses et risques

| Risque | Réponse |
|-|-|
| Calculs fiscaux dépendant des évolutions réglementaires. | Mise en place de boutons ouvrant directement les pages Internet des services publics consacrées à ces informations. |
| Évolution des pratiques locatives du client (paiement ou non de l'eau, puis régularisation en fin d'année). | Mise en place de formulaires flexibles, avec des champs explicites par domaine (loyer, eau, ordures, etc.). |
| Bouleversement des habitudes du client par le nouveau modèle de processus, en cas d'accompagnement insuffisant. | Mise en place d'une méthode agile informelle et présentation progressive des fonctionnalités, accompagnées de démonstrations et d'une prise en main adaptée aux pratiques du client. |
| Abandon du logiciel par le client s'il rencontre trop de difficultés à l'utiliser. | Maintien d'un logiciel simple et visuel, avec démonstration de la valeur ajoutée apportée à son activité, notamment en matière de gain de temps et de réduction de la charge cognitive. |

## 9 – Évolutions prévues

**Ouverture du projet :**

Une version open source du projet a été publié, améliorée par rapport à la version originale publiée sur GitHub.

Volonté de rendre le logiciel évolutif et réutilisable.

**Découplage du backend :**

Au cours du développement, l'auteur a isolé la gestion des bases de données et créé un nouveau dépôt GitHub (Ligarium).

L'objectif est de séparer la logique métier applicative de la gestion des données SQL, cette dernière reposant sur des opérations et des mécanismes communs, quel que soit le contexte d'utilisation.

- Permettre la modification et la maintenance de Rentalis de manière rapide et modulaire, sans impacter la couche de gestion des données.

**Objectif final :**

Remplacement du backend actuel de Rentalis par la bibliothèque Ligarium.

> **Note :** Rentalis est actuellement en pause, le temps de rendre Ligarium suffisamment fiable pour être intégré au projet. Le client dispose d'une version stable de Rentalis pendant cette période de développement et de consolidation du backend.

## 10 – Glossaire

- **Bâtiment :** bien immobilier appartenant à un ou plusieurs propriétaires, pouvant accueillir un ou plusieurs locataires, comporter plusieurs pièces et être associé à des dommages, des réparations et des documents.
- **Locataire :** personne physique ou morale titulaire d'un bail portant sur l'un des bâtiments.
- **Propriétaire :** personne physique ou morale possédant un ou plusieurs bâtiments. Dans le contexte initial, il s'agit du client. Le logiciel doit toutefois permettre d'envisager plusieurs propriétaires au sein d'une même base de données.
- **Quittance :** document remis par le propriétaire au locataire pour attester du paiement intégral du loyer et des charges pour une période donnée.
- **Facture :** document remis par le propriétaire au locataire pour formaliser une régularisation des charges, notamment en fin d'année. Selon les pratiques du propriétaire, elle peut notamment concerner l'eau, les ordures ménagères, etc.
- **Loyer :** montant du loyer, charges comprises.
- **Dommage :** détérioration constatée sur un bien immobilier (usure, accident, destruction, etc.).
- **Réparation :** intervention réalisée à la suite d'un dommage afin de remettre un bien en état.

## 11 – Livrables

- Application installable sous Windows et fonctionnelle hors ligne.
- Migration des données du client depuis Excel vers une base de données compatible avec l'application.
- Copie de la base de données migrée au format `.sqlite`, importable dans l'application.
- Démonstration complète et accompagnement à la prise en main.
