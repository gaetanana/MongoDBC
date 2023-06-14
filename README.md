# Application MongoDB en C

## Guide de configuration du projet

### Prérequis

- Avoir le dossier compressé "**MongoDBC.zip**" (Vous devez forcément avoir le fichier ZIP, car dans le dépôt GitHub, il n'y a pas le driver).

Voici le lien du fichier ZIP du projet : https://drive.google.com/file/d/1TugkuCtDq_Uv2fcGbVKgxX6McpF0cH0c/view?usp=sharing

- Avoir l'IDE **Visual Studio 2019**.
- Un serveur MongoDB qui tourne (vous trouverez un tutoriel à l'emplacement suivant : https://github.com/gaetanana/MongoDBC/blob/main/Documentation/InstallationServeurMongoDB.pdf.
- Avoir installer le compilateur **Visual Studio 2019**.
- Vous devrez avoir les fichiers binaires dans vos variables d'environnement.

### Fichiers binaires dans les variables d'environnement

Pour que le projet fonctionne, vous devez avoir les fichiers binaires des dépendances dans vos variables d'environnement.

Voici les chemins des fichiers binaires :

`VOTRE CHEMIN`\MongoDBC\Dependances\DriverJSON\jsoncpp2\build\install\bin

`VOTRE CHEMIN`\MongoDBC\Dependances\DriverMongoDB\mongo-c-driver-1.22.1\mongo-c-driver-1.22.1\build\install\bin

`VOTRE CHEMIN`\MongoDBC\Dependances\DriverXML\bin

`VOTRE CHEMIN`\MongoDBC\Dependances\MSVRC100

`VOTRE CHEMIN`\MongoDBC\Dependances\Zlib

Pour ajouter ces chemins dans vos variables d'environnement, vous devez suivre les étapes suivantes :

1. Tapez dans la barre de recherche de votre PC "**variables d'environnement**" et cliquez sur "**Modifier les variables d'environnement système**".

![variableEnvironnement.png](Documentation%2FimgREADME%2FBINAIRE%2FvariableEnvironnement.png)

2. Cliquez sur "**Variables d'environnement**" _(surligné sur la capture d'écran en bleu)_.

![FenetreVariable1.png](Documentation%2FimgREADME%2FBINAIRE%2FFenetreVariable1.png)

3. Dans la fenêtre qui s'ouvre, sélectionnez la variable "**Path**" _(surligné sur la capture d'écran en rose)_ et cliquez sur "**Modifier**".

![FenetreVariable2.png](Documentation%2FimgREADME%2FBINAIRE%2FFenetreVariable2.png)

4. Dans la fenêtre qui s'ouvre, cliquez sur "**Nouveau**" _(surligné sur la capture d'écran en vert)_ et ajoutez les chemins des fichiers binaires.

![FenetreVariable3.png](Documentation%2FimgREADME%2FBINAIRE%2FFenetreVariable3.png)

Une fois que vous avez ajouté les chemins des fichiers binaires, vous pouvez fermer toutes les fenêtres.

Redémarrez votre PC pour que les changements soient pris en compte.

### Configuration du projet avec Visual Studio 2019

Maintenant que vous avez les fichiers binaires dans vos variables d'environnement, vous pouvez configurer le projet avec Visual Studio 2019.

1. Décompresser le dossier "**MongoDBC.zip**" dans un dossier de votre choix.

![ExtractionFichierZIP.png](Documentation%2FimgREADME%2FVISUALSTUDIO%2FExtractionFichierZIP.png)

2. Ouvrir le projet avec Visual Studio (dossier MongoDBC).

![OuvertureProjetVisualSTUDIO.png](Documentation%2FimgREADME%2FVISUALSTUDIO%2FOuvertureProjetVisualSTUDIO.png)

À ce stade, vous devriez avoir les messages d'erreurs suivantes :

![ecranVISUALSTUDIO.png](Documentation%2FimgREADME%2FVISUALSTUDIO%2FecranVISUALSTUDIO.png)
C'est normal, car par défaut Visual Studio 2019 choisit la version 64 bits pour compiler le projet.
Il faut donc changer la configuration du projet pour qu'il tourne en 32 bits.

3. Pour changer la configuration du projet, vous devez modifier la configuration qui est
à côté du bouton d'exécution du projet, vous devez faire défiler et cliquez sur "**Gérez les configurations..**".

![ConfigurationVersion.png](Documentation%2FimgREADME%2FVISUALSTUDIO%2FConfigurationVersion.png)

4. Supprimez la configuration "**x64**" en la sélectionnant et en cliquant sur la croix rouge.

![version1.png](Documentation%2FimgREADME%2FVISUALSTUDIO%2Fversion1.png)

5. Ajoutez une nouvelle configuration en cliquant sur la croix verte.

![AjoutConfiguration.png](Documentation%2FimgREADME%2FVISUALSTUDIO%2FAjoutConfiguration.png)

6. Dans la fenêtre qui s'ouvre, sélectionnez "**x86-Debug**" et cliquez sur "**Sélectionner**", faite le même chose pour **x86-Release**.

7. Enfin executez la classe **main.c** en cliquant sur la flèche verte.

![resultat.png](Documentation%2FimgREADME%2FVISUALSTUDIO%2Fresultat.png)



Une console s'ouvre et affiche le résultat de l'exécution du programme.

### Contenue de l'application

L'application contient 4 dossiers qui contiennent chacun un fichier :

- **CREATE** : Contient le fichier **create.c** qui contient toutes les fonctions qui permettent de simuler les requêtes de création de données dans la base de données.

Les fonctions disponibles dans ce fichier sont les suivantes :

**Fonction n°1 :** `void createCollection(mongoc_client_t *client,const char *db_name,const char* nomCollection)`

Cette fonction permet de créer une collection dans la base de données qui est passé en paramètre de la fonction.

**Fonction n°2 :** `void insert_document_into_collection (mongoc_client_t *client,const char *db_name, const char *collection_name,const char* value)`

Cette fonction permet d'insérer un document dans une collection et une base de données qui sont passées en paramètre de la fonction.

**Fonction n°3 :** `void insert_xml_into_collection(mongoc_client_t *client,const char *db_name, const char *collection_name,const char* pathXML)`

Cette fonction permet d'insérer un document XML dans une collection et une base de données qui sont passées en paramètre de la fonction.

**Fonction n°4 :** `void process_files_one_by_one(mongoc_client_t *client,const char *db_name,const char *collection_name,const char* dirPath)`

Cette fonction permet d'insérer tous les documents XML d'un dossier dans une collection et une base de données qui sont passées en paramètre de la fonction.
Mais cette fonction insère les documents un par un.


**Fonction n°5 :** `void process_all_files_at_once(mongoc_client_t *client,const char* db_name,const char* collection_name,const char* dirPath)`

Cette fonction permet d'insérer tous les documents XML d'un dossier dans une collection et une base de données qui sont passées en paramètre de la fonction.
Mais cette fonction charge les fichiers XML au préalable dans un tableau, puis insère tous les documents en une seule fois.

REPRENDRE ICI

- **READ** : Contient le fichier **read.c** qui contient toutes les fonctions qui permettent de simuler les requêtes de lecture de données dans la base de données.

Les fonctions disponibles dans ce fichier sont les suivantes :


- **UPDATE** : Contient le fichier **update.c** qui contient toutes les fonctions qui permettent de simuler les requêtes de mise à jour de données dans la base de données.



- **DELETE** : Contient le fichier **delete.c** qui contient toutes les fonctions qui permettent de simuler les requêtes de suppression de données dans la base de données.

Un fichier **main.c** est présent, englobant toutes les fonctions de l'application. Il est complété par une interface utilisateur en console, qui permet d'exploiter ces fonctions.

Un fichier **CmakeList.txt** est présent, il permet de build le projet.

### Informations complémentaires

Pour l'instant, l'application ne possède pas de console interactive. 
Cela est en cours de développement. 
Cependant, en attendant, vous pouvez tester les fonctions déjà présentes dans le programme en 
les commentant et en les décommentant.

Actuellement, dans le main vous avez toutes les fonctions disponibles et il y 3 arguments qui sont obligatoires :

1) Le client MongoDB (qui est le point d'entrée pour accéder à la base de données).
2) Le nom de la base de données.
3) Le nom de la collection.

Ces arguments, vous pouvez les changer dans le main, selon vos besoins.


![captureFonctions.png](Documentation%2FimgREADME%2FVISUALSTUDIO%2FcaptureFonctions.png)
