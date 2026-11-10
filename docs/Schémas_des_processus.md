# Schémas des processus

Les processus les plus laborieux sont présentés ici afin de mettre en évidence la valeur ajoutée du projet.

## Envoi des factures

**Ancien processus :**

Il comprend quatre environnements de travail distincts, dont un environnement physique :
- Excel
- Calculs effectués par le client (calculatrice, papier et crayon)
- Logiciel de traitement de texte
- Boîte mail

```mermaid
flowchart TB
AA{Besoin d'envoyer une facture} --> A
A(Ouverture d'Excel) --> B(Recherche des loyers par locataire, date et bâtiment)
B -- Changement d'environnement de travail --> C(Calcul des charges et des loyers)
C -- Changement d'environnement de travail --> D(Création d'un document texte)
C -. Report des calculs .-> D
D --> E(Rédaction de la facture)
C -. Report des données .-> E
E --> G(Sauvegarde de la facture)
G -- Changement d'environnement de travail --> H(Ouverture de la boîte mail)
H --> I(Rédaction du mail)
C -. Report du mail et du nom du locataire .-> I
I --> J(Ajout des pièces jointes)
J --> K(Envoi du mail)
K --> L{Facture reçue par le locataire}
```

**Nouveau processus :**

Il ne comprend plus que deux environnements de travail distincts, tous deux numériques :
- Le logiciel Rentalis
- La boîte mail

```mermaid
flowchart TB
AA{Besoin d'envoyer une facture} --> A
A(Ouverture du logiciel) --> B(Clic sur Menu Factures > Nouvelle facture)
B --> C(Saisie du locataire et de la date)
C --> D(Clic sur Enregistrer la facture > Envoyer par mail)
C -. Génération des données .-> D
D -- Changement d'environnement de travail --> E
E(Inspection du mail > Ajout des pièces jointes indiquées > Clic sur Envoyer)
D -. Génération des données mailto .-> E
E --> L{Facture reçue par le locataire}
```

**Gains : (estimations auteur)**

| Critère | Avant | Après |
|---|---:|---:|
| Environnements de travail | 4 | 2 |
| Nombre de clics | > 30 | ~ 10 |
| Processus entièrement numérique | Non | Oui |
| Nombre de saisies | > 4 | ~ 2 |
| Nombre de rédactions | 2 | 0 |
| Format des pièces jointes | Imprévisible | `.pdf` |

## Envoi des quittances

**Ancien processus :**

Il comprend cinq environnements de travail distincts, dont trois environnements physiques :
- Excel
- Calculs effectués par le client (calculatrice, papier et crayon)
- Rédaction dans le carnet de quittances
- Numérisation du carnet de quittances
- Boîte mail

```mermaid
flowchart TB
AA{Besoin d'envoyer une quittance} --> A
A(Ouverture d'Excel) --> B(Recherche des loyers par locataire, date et bâtiment)
B -- Changement d'environnement de travail --> B1(Calculs divers)
B1 --> C(Récupération du carnet de quittances)
C -- Changement d'environnement de travail --> D(Report manuscrit des données)
D -- Changement d'environnement de travail --> E(Numérisation du carnet)
E -- Changement d'environnement de travail --> G(Récupération de l'image numérisée)
G --> H(Ouverture de la boîte mail)
H --> I(Rédaction du mail)
B -. Report du mail et du nom du locataire .-> I
I --> J(Ajout de l'image numérisée en pièce jointe)
J --> K(Envoi du mail)
K --> L{Quittance reçue par le locataire}
```

**Nouveau processus :**

Il ne comprend plus que deux environnements de travail distincts, tous deux numériques :
- Le logiciel Rentalis
- La boîte mail

```mermaid
flowchart TB
AA{Besoin d'envoyer une quittance} --> A
A(Ouverture du logiciel) --> B(Clic sur Menu Quittances > Nouvelle quittance)
B --> C(Saisie du locataire et de la date)
C --> D(Clic sur Enregistrer la quittance > Envoyer par mail)
C -. Génération des données .-> D
D -- Changement d'environnement de travail --> F(Inspection du mail > Ajout des pièces jointes indiquées > Clic sur Envoyer)
D -. Génération des données mailto .-> F
F --> L{Quittance reçue par le locataire}
```

**Gains : (estimations auteur)**

| Critère | Avant | Après |
|---|---:|---:|
| Environnements de travail | 5 | 2 |
| Nombre de clics | > 20 | ~ 10 |
| Processus entièrement numérique | Non | Oui |
| Nombre de saisies | > 4 | ~ 2 |
| Nombre de rédactions | 2 | 0 |
| Format des pièces jointes | Imprévisible | `.pdf` |
