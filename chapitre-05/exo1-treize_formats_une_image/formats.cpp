#include "NKImage/Core/NkImage.h"
#include <filesystem>
#include <fstream>
#include <iostream>

using namespace nkentseu;
namespace fs = std::filesystem;

int main() {
    NkImage photo;
    const std::string sourceImage = "photo.jpg";

    // 1. Chargement de l'image originale du mur
    if (!photo.Load(sourceImage.c_str())) {
        std::cerr << "Erreur : impossible de charger " << sourceImage << ;
        return 1;
    }

    const int wOrig = photo.Width();
    const int hOrig = photo.Height();

    // Structure pour associer chaque format à sa méthode d'écriture
    struct Format {
        std::string nom;
        std::string chemin;
        bool (NkImage::save)(const char) const noexcept;
        bool customSave; // Vrai pour le JPEG (nécessite un paramètre de qualité)
    };

    // Définition des 5 formats demandés par l'exercice
    Format formats[] = {
        {"PNG",  "photo_sortie.png", &NkImage::SavePNG, false},
        {"JPEG", "photo_sortie.jpg", nullptr,           true},
        {"BMP",  "photo.bmp",        &NkImage::SaveBMP, false},
        {"TGA",  "photo.tga",        &NkImage::SaveTGA, false},
        {"QOI",  "photo.qoi",        &NkImage::SaveQOI, false}
    };

    // Ouverture du fichier de sortie pour enregistrer les résultats
    std::ofstream resultat("formats.txt");
    if (!resultat) {
        std::cerr << "Erreur : impossible de creer formats.txt.";
        return 1;
    }

    // 2. Traitement itératif pour chaque format d'image
    for (const auto& f : formats) {
        // Écriture du fichier selon le type (standard ou avec paramètre de qualité)
        bool ecriture = f.customSave ? photo.SaveJPEG(f.chemin.c_str(), 90)
                                      : (photo.*f.save)(f.chemin.c_str());

        // Récupération de la taille du fichier sur le disque en octets
        std::uintmax_t taille = 0;
        std::error_code err;
        if (ecriture && fs::is_regular_file(f.chemin, err)) {
            taille = fs::file_size(f.chemin, err);
        }

        // 3. Relecture du fichier généré pour vérifier sa validité
        NkImage imgRelue;
        bool lecture = ecriture && imgRelue.Load(f.chemin.c_str());

        // 4. Inscription des résultats (taille et dimensions) dans formats.txt et la console
        if (lecture) {
            resultat << f.nom << " : " << taille << " octets, " 
                     << imgRelue.Width() << " x " << imgRelue.Height() <<;
            std::cout << f.nom << " : " << taille << " octets, " 
                      << imgRelue.Width() << " x " << imgRelue.Height() <<;
        } else {
            resultat << f.nom << " : " << taille << " octets, illisible ";
            std::cout << f.nom << " : " << taille << " octets, illisible ";
        }
    }

    // Affichage des dimensions de référence de l'image de départ
    std::cout << "Dimensions originales : " << wOrig << " x " << hOrig << ;
    
    return 0;
}
