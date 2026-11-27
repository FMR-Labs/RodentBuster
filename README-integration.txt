RodentBuster - extraction initiale GUI / DSP / plugin iPlug2

Fichiers :
- RodentBuster.h/.cpp       : plugin iPlug2, paramètres VST3 et liaison host.
- RodentBusterDSP.h/.cpp    : moteur audio.
- RodentBusterGUI.h/.cpp    : construction de l'interface graphique.

CMakeLists.txt :
Ajouter les deux nouveaux fichiers :
    RodentBusterDSP.cpp
    RodentBusterGUI.cpp

Conserver RodentBuster.cpp dans les sources du plugin.

Hypothèses reprises du projet d'origine :
- KNOB_FN, SWITCH_FN, BACKGROUND_FN et les trois macros de polices sont
  définies par la configuration du plugin.
- Les chemins relatifs vers FMR_Library sont identiques à ceux du projet source.
- ND_RODENT_BUSTER_NB_CHANNELS est défini par FMR_RodentBuster.h.
- Les indices de paramètres et les tags des contrôles sont conservés.

Notes :
- IRotatedTextControl et Conv_Pot n'étaient pas utilisés dans le code fourni,
  ils ne sont donc pas déplacés.
- La fonction ProcessBlock continue à utiliser les mêmes valeurs de paramètres,
  le même resampling, le même lissage, les mêmes appels au moteur et le même
  calcul des crêtes.
- Cette extraction est une première version à compiler dans le projet réel :
  les chemins de ressources et les types exacts de la version iPlug2 doivent
  être validés par le build.
