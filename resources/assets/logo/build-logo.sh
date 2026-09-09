#!/bin/bash

# Vérifie que le fichier rentalis.svg existe
if [ ! -f "rentalis.svg" ]; then
    echo "Erreur : rentalis.svg introuvable dans le dossier courant."
    exit 1
fi

# Définir les tailles souhaitées
sizes=(16 24 32 48 64 128)

# Convertir le SVG en PNG pour chaque taille
for size in "${sizes[@]}"; do
    echo "Création de rentalis-$size.png..."
    convert -background none -resize "${size}x${size}" rentalis.svg "rentalis-$size.png"
done

# Créer le fichier ICO à partir des PNG
echo "Création de rentalis.ico..."
convert rentalis-16.png rentalis-24.png rentalis-32.png rentalis-48.png rentalis-64.png rentalis-128.png rentalis.ico

echo "Terminé !"

