# Schéma des processus

Ici les processus les plus laborieux sont indiqué afin de démontrer l'intérêt du projet

## Envoi des factures 

**Ancien processus :**

Comprend 4 environnements de travail distincts dont 1 physique :
* Excel
* Calcul côté client (calculatrice, papier crayon)
* Logiciel d'édition de texte
* Boîte mail

```mermaid
flowchart TB
AA{Besoin Envoie Facture} --> A
A(Ouverture Excel) --> B(Recherche loyers par locataire par date et bâtiment)
B -- changement environnement de travail -->C(Calcul charges et loyers)
C -- changement environnement de travail --> D(Création document texte)
C -. Report des calculs .-> D
D --> E(Rédaction de la facture)
C -. Report des données .-> E
E --> G(Sauvegarde facture)
G -- changement environnement de travail --> H(Ouverture boîte mail)
H --> I(Rédaction mail)
C -. Report mail et nom du locataire .-> I
I --> J(Ajout pièces jointes)
J --> K(Envoi mail)
K --> L{Facture reçut par le locataire}
```

**Nouveau processus :**

Ne comprend plus que 2 environnements de travail distincts, tous numériques :
* Le logiciel Rentalis
* Boîte mail
```mermaid
flowchart TB
AA{Besoin Envoie Facture} --> A
A(Ouverture Logiciel) --> B(clic Menu Factures > Nouvelle Facture)
B --> C(Saisie champ locataire, champ date)
C --> D(clic enregistrer facture > envoyer par mail)
C -. génération des données .-> D
D -- changement environnement de travail --> E
E(Inspection mail > ajout des pièces jointes indiquées > clic Envoyer)
D -. génération des données mailto .-> E
E --> L{Facture reçut par Locataire}
```

**gains :**
| critère | avant | après
|-|-|-|
| envrionnements de travail | 4 | 2 |
| nombre de clics | >30 | ~ 10 | 
| totalement numérique | non | oui |
| nombre de saisies | > 4 | ~ 2 |
| nombre de rédactions | 2 | 0 |
| format pièces jointes | imprévisible | `.pdf` |

## Envoi des quittances

**Ancien processus :**

Comprend 5 environnements de travail distincts dont 3 physiques :
* Excel
* Calcul côté client (calculatrice, papier crayon)
* Rédaction dans le carnet de quittance
* Scanner le carnet de quittance
* Boîte mail

```mermaid
flowchart TB
AA{Besoin Envoie Quittance} --> A
A(Ouverture Excel) --> B(Recherche Loyers par locataire par date et bâtiment)
B -- changement environnement de travail -->B1(Calcul divers)
B1 --> C(Récupération carnet de quittance)
C -- changement environnement de travail --> D(Report manuscrite des données)
D -- changement environnement de travail --> E(Numérisation du carnet)
E -- changement environnement de travail --> G(Récupération image numérisée)
G --> H(Ouverture boîte mail)
H --> I(Rédaction mail)
B -. Report mail et nom du locataire .-> I
I --> J(Ajout pièces jointes image numérisée)
J --> K(Envoi mail)
K --> L{Quittance reçut par le locataire}
```

**Nouveau processus :**

Ne comprend plus que 2 environnements de travail distincts, tous numériques :

```mermaid
flowchart TB
AA{Besoin Envoie Quittance} --> A
A(Ouverture Logiciel) --> B(clic Menu Quittances > Nouvelle Quittance)
B --> C(Saisie champ locataire, champ date)
C --> D(clic enregistrer quittance > envoyer par mail)
C -. génération des données .-> D
D -- changement environnement de travail --> F(Inspection mail > ajout des pièces jointes indiquées > clic Envoyer)
D -. génération des données mailto .-> F
F --> L{Quittance reçut par le locataire}
```


**gains :**
| critère | avant | après
|-|-|-|
| envrionnements de travail | 5 | 2 |
| nombre de clics | >20 | ~ 10 |
| totalement numérique | non | oui |
| nombre de saisies | > 4 | ~ 2 |
| nombre de rédactions | 2 | 0 |
| format pièces jointes | imprévisible | `.pdf` |
