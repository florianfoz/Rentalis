# Journal des évolutions

Documentation rétrospective, rédigée a posteriori à partir du projet d'origine.

Présentation des principales évolutions du projet et des changements apportés.

Les dates sont données à titre indicatif. Elles représentent uniquement le temps écoulé entre les évolutions, et non le temps de développement effectif.

## E-00 – Définition des outils

01/05/2025

Utilisation du framework Qt C++.

- Multiplateforme (client sous Windows, développeur sous Linux).
- Langage C++ maîtrisé par le développeur.
- Licence compatible avec le projet.
- Framework éprouvé et reconnu.
- Stabilité et performances.
- Communauté active et bibliothèques nombreuses.

## E-01 – Version initiale

01/06/2025

**Fonctionnalités**
- Mise en place des principaux domaines fonctionnels de la gestion locative.
- Mise en place du modèle de base de données SQL et du code ORM.
- Centralisation des informations relatives aux bâtiments, locataires, propriétaires, loyers, etc.
- Gestion des loyers, bâtiments, locataires et propriétaires.
- Modification de la base de données en temps réel.

**Métier**
- Adaptation aux pratiques du client.
- Simplification des processus de saisie et de consultation des données.

## E-02 – Quittances et factures

15/06/2025

**Fonctionnalités**
- Mise en place de la gestion des quittances et des factures liées aux locataires et aux propriétaires, selon une période donnée et les loyers payés.

**Métier**
- Standardisation des quittances et des factures du client.

> US-01

## E-03 – Impressions

01/07/2025

**Fonctionnalités**
- Mise en place de l'exportation au format `.pdf` des factures et des quittances, à partir de formulaires préremplis et datés.

**Métier**
- Archivage des fichiers générés par le client.

## E-04 – E-mails

20/07/2025

**Fonctionnalités**
- Mise en place de la préparation des e-mails d'envoi des quittances et des factures aux locataires, avec un contenu prérempli et contextualisé grâce à l'intégration de `mailto:`.

**Métier**
- Envoi rapide d'e-mails professionnels.
- Classement des e-mails par date et par client.

> US-02

## E-05 – Champs de données strictement contrôlés

08/08/2025

**Fonctionnalités**
- Mise en place de filtres, de règles et de vérifications sur les champs éditables (texte, numérique, etc.).
- Mise en place de certaines règles de cohérence des données (exemple : date de début < date de fin).
- Mise en place d'alertes en cas de modifications importantes.

> US-03

## E-06 – Extraction des données (Dump)

02/10/2025

**Fonctionnalités**
- Conversion des données ciblées en texte.
- Mise en contexte et formatage sous forme de prompt.
- Copie du texte formaté dans le presse-papiers.

**Métier**
- Utilisation contrôlée de l'IA dans le cadre de l'activité.

> US-04

## E-07 – Sauvegardes versionnées

25/10/2025

**Fonctionnalités**
- Sauvegarde par copie de la base de données, avec date, description et catégorie.
- Choix du mode de sauvegarde : création d'une nouvelle sauvegarde ou écrasement des données actuelles.
- Mise en place de la sauvegarde automatique.
- Alerte en cas de fermeture sans sauvegarde préalable.

**Métier**
- Archivage mensuel des données par mesure de sécurité.
- Modifications effectuées plus sereinement.

> US-05

## E-08 – Amélioration du tableau de bord

10/11/2025

**Fonctionnalités**
- Amélioration du tableau de bord avec des filtres et des données complémentaires.
- Mise en place de graphiques dans le tableau de bord et dans la section des loyers.

**Métier**
- Mesure de la rentabilité par bâtiment et identification des périodes les plus profitables.
- Identification des bons et des mauvais payeurs.
- Mesure de l'amortissement des coûts de réparation et d'entretien des bâtiments.

> US-06

## E-09 – Fiscalité

20/11/2025

**Fonctionnalités**
- Ajout de panneaux de calcul fiscal.
- Ajout de boutons hyperliens vers les sites gouvernementaux relatifs à la réglementation fiscale immobilière.

**Métier**
- Simplification des calculs et des démarches fiscales.

> US-07

## E-10 – Livraison finale

05/12/2025

Logiciel considéré comme complet et opérationnel pour le client.

Les prochaines versions seront consacrées à l'ouverture du code source. Le client pourra choisir d'utiliser la version open source s'il le souhaite.

## E-11 – Préparation à l'ouverture du code source

05/06/2026

Nettoyage et standardisation du code.

**Fonctionnalités**
- Traduction en anglais.
- Mise en place d'un système de gestion des langues.
- Mise en place d'un système de thèmes CSS.
- Paramétrage de la localisation.
- Ajout d'un menu « À propos ».
- Ajout d'un menu d'accueil.

Révision ou suspension de certaines fonctionnalités trop spécifiques :
- Fiscalité.
- Langue française codée en dur dans les interfaces.

## E-12 – Ouverture du code source à la communauté open source

09/09/2026

- Définition de la licence.
- Rédaction de la documentation initiale.
- Publication du dépôt.

## E-13 – Séparation du backend SQL

21/09/2026

- Extraction du code métier relatif à la gestion de la base de données SQL.
- Création du dépôt [Ligarium](https://github.com/florianfoz/Ligarium).
- Mise en pause de Rentalis le temps de finaliser Ligarium.
- Généralisation du code de Ligarium.

> Évolution actuelle

## E-14 - Publication des documentations du projet
10/11/2026

- Publication de la note de cadrage
- Publication du journal des évolutions
- Publication du cahier des charges
- Publication des schémas des processus
- Publication de user stories
