# Cahier des charges

Documentation rétrospective, rédigée a posteriori à partir du projet d'origine.

Cela concerne la version livrée au client.

Certaines priorités diffèrent, en raison de l'évolution des besoins à l'usage, visible dans les user stories.

| ID | Exigence | Priorité | Source |
|-|-|-|-| 
| EF-01 | Le logiciel doit gérer les bâtiments. | Must | E-01, US-00, Note de cadrage § 3-1 |
| EF-02 | Le logiciel doit gérer les loyers. | Must | E-01, US-00, Note de cadrage § 3-1 |
| EF-03 | Le logiciel doit gérer les locataires. | Must | E-01, US-00, Note de cadrage § 3-1 |
| EF-04 | Le logiciel doit gérer les propriétaires. | Must | E-01, US-00, Note de cadrage § 3-1 |
| EF-05 | Le logiciel doit gérer les dommages. | Must | E-01, US-00, Note de cadrage § 3-1 |
| EF-06 | Le logiciel doit gérer les réparations. | Must | E-01, US-00, Note de cadrage § 3-1 |
| EF-07 | Le logiciel doit générer une quittance au format PDF non modifiable pour un locataire et une période donnés. | Must | US-01, E-02, E-03 |
| EF-08 | Le logiciel doit générer une facture au format PDF non modifiable pour un locataire et une période donnés. | Must | US-01, E-02, E-03 |
| EF-09 | Le logiciel doit préparer un e-mail prérempli (destinataire, objet, formule de politesse) à partir d'une quittance ou d'une facture et indiquer les pièces jointes. | Must | US-02, E-04 |
| EF-10 | Le logiciel doit assurer l'intégrité de la base de données lors d'opérations dangereuses, telles que les suppressions. | Must | US-03, E-05 |
| EF-11 | Le logiciel doit assurer la cohérence et la mise en forme des données saisies dans les champs prévus à cet effet. | Must | US-03-1, E-05 |
| EF-12 | Le logiciel doit permettre de sauvegarder différentes bases de données à la demande, avec une date, une description et une catégorie, ou de réaliser des sauvegardes automatiques. | Could | US-05, E-07 |
| EF-13 | Le logiciel doit proposer une interaction simplifiée avec n'importe quel chatbot d'IA, tout en garantissant l'anonymisation des données transmises et le respect de la vie privée. | Should | US-04, E-06 |
| EF-14 | Le logiciel doit afficher une vue globale de l'activité sous la forme d'un tableau de bord comprenant des filtres et des graphiques. | Should | US-06, E-08, Note de cadrage § 3-3 |
| EF-15 | Le logiciel doit calculer certains paramètres fiscaux et faciliter les déclarations. | Could | US-07 |
| ENF-01 | Le logiciel doit fonctionner sans connexion Internet sous Windows 11. | Must | Note de cadrage § 7, E-00 |
| ENF-02 | Les données doivent pouvoir être exportées dans un format indépendant du logiciel. | Must | Note de cadrage § 3-3 |
| ENF-03 | Le logiciel doit être relativement léger et réactif. | Must | Note de cadrage § 7, E-00 |
| ENF-04 | Le logiciel doit être ergonomique, permettre d'effectuer des opérations en quelques clics et demeurer relativement simple d'utilisation. | Must | Note de cadrage § 3-2, Schémas des processus |
