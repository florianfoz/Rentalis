# User Stories

Chronique synthétique et rétrospective du développement du projet, à travers les interactions entre le client et l'auteur.

## US-00 - Créer une solution numérique à l'activité

En tant que client, je veux bénéficier d'une solution numérique pour mes processus, afin de me concentrer sur le développement de mon activité. 

**Critères d'acceptation :**
- Numérisation de tous les processus.
- Gestion des bâtiments, loyers, locataires, propriétaires, dommages, réparations.
- Logiciel viable et utilisable sur Windows 11 sans connexion Internet.

> Voir: Note de cadrage

## US-01 – Générer une quittance (Must)

En tant que propriétaire, je veux générer une quittance à partir d'un paiement de loyer enregistré, afin de ne plus avoir à la rédiger à la main.

**Critères d'acceptation :**
- La quittance reprend automatiquement les données du locataire, du bail et de la période concernée. Elle est exportée dans un format portable et non modifiable (`.pdf`).
- L'utilisateur peut ajouter des informations complémentaires si nécessaire.
- L'opération s'effectue en moins de 5 clics, en renseignant le locataire et la période.

## US-02 – Envoyer des e-mails (Must)

En tant que propriétaire, je veux envoyer des e-mails à mes locataires afin de leur communiquer rapidement et facilement les factures et les quittances.

**Critères d'acceptation :**
- L'e-mail reprend automatiquement les données du locataire, les formules de politesse, l'objet, la nature du message et les pièces jointes à ajouter.
- L'utilisateur peut ajouter des informations complémentaires à l'e-mail avant de l'envoyer depuis sa boîte mail habituelle.
- L'opération s'effectue en moins de 5 clics à partir de la facture ou de la quittance concernée.
- L'e-mail est envoyé automatiquement (Won't).
  - Voir US-02-01.

## US-02-01 – Rejet de l'envoi automatique des e-mails (Won't)

En tant que développeur, je veux conserver un logiciel relativement simple et cohérent afin d'assurer sa fiabilité et de répondre aux exigences principales du client. 

**Justification :**
- L'e-mail est prérempli à l'aide d'un lien `mailto:` et indique les pièces jointes à ajouter.
- Cette fonctionnalité nécessiterait l'utilisation des API des fournisseurs de messagerie ou d'un autre mécanisme d'envoi.

## US-03 – Intégrité des données (Must)

En tant qu'utilisateur, je veux disposer d'une base de données fiable afin d'éviter toute corruption de mes données.

**Critères d'acceptation :**
- Présence de boutons explicites `Appliquer`, `Confirmer`, `Enregistrer` et `Supprimer` pour valider les modifications.
- Affichage de messages d'alerte en cas d'opérations dangereuses ou de suppression (Must).

## US-03-01 – Problèmes de saisie (Must)

En tant que développeur, je veux m'assurer que le client renseigne correctement les champs afin de garantir la cohérence de la base de données.

**Justifications :**
- Les formulaires d'enregistrement vérifient certains champs afin d'assurer la cohérence des données selon le contexte (dates, locataire, etc.) (Should).
- Les champs sont paramétrés afin de restreindre les formats de données autorisés (prix, valeurs numériques, texte, etc.).

## US-04 – IA d'assistance (Should)

En tant qu'utilisateur, je veux pouvoir utiliser des IA dans le cadre de mon activité, tout en gardant le contrôle sur les informations que je leur transmets, afin d'exercer mon activité avec l'assistance de l'IA, tout en respectant ma vie privée et celle de mes clients.

**Critères d'acceptation :**
- Aucune IA n'est intégrée directement au logiciel et aucune dépendance externe à un service d'IA n'est imposée (Must).
- Des prompts sont générés en moins de 3 clics à partir de contextes de données précis.
- Les données transmises sont pseudonymisées, les données numériques et dates sont transmises (Must).

## US-05 – Oubli des sauvegardes (Could)

En tant qu'utilisateur, je veux pouvoir utiliser l'application sans me soucier des sauvegardes, afin de travailler sereinement tout en conservant correctement mes données.

**Critères d'acceptation :**
- Possibilité de paramétrer les sauvegardes automatiques.
- Demande de sauvegarde et alerte en cas de risque de perte de données à la fermeture (Must).

## US-06 – Vue globale (Should)

En tant que propriétaire, je veux disposer d'une vue globale de mon activité, organisée par bâtiment et par période, afin d'avoir une vision d'ensemble de ma gestion immobilière.

**Critères d'acceptation :**
- Présence d'un tableau de bord comprenant des indicateurs et des filtres paramétrables.

## US-07 – Fiscalité (Could)

En tant que propriétaire, je veux pouvoir calculer certains paramètres fiscaux afin de déclarer correctement mon activité auprès de l'État.

**Critères d'acceptation :**
- Système de calcul adapté à un régime fiscal.
- Remplissage des documents fiscaux (Won't).
  - Voir US-07-01.

## US-07-01 – Rejet du remplissage des documents fiscaux (Won't)

En tant que développeur, je veux délimiter clairement le périmètre fonctionnel couvert par le logiciel afin de conserver une solution viable et pertinente.

**Justifications :**
- Présence de raccourcis vers les sites officiels de l'État concernant les déclarations et les régimes fiscaux.
- Ayant étudié le droit fiscal durant ma formation universitaire, mieux vaut éviter de générer des déclarations erronées à l'aide d'un logiciel qui ne serait pas à jour au regard des dernières réglementations de l'État. Par ailleurs, l'accès aux formulaires de déclaration fiscale implique le traitement de données sensibles, auquel le logiciel n'est pas destiné.
