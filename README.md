# Application MongoDB en C

## Guide de configuration du projet

### Prérequis

- Avoir le dossier compressé "**MongoDBC.zip**" (Vous devez forcément avoir le fichier ZIP, car dans le dépôt GitHub, il n'y a pas le driver).
- Avoir l'IDE **Visual Studio 2019**.
- Un serveur MongoDB qui tourne (vous trouverez un tutoriel à l'emplacement suivant : [Cliquez ici](./../MongoDBC/Documentation/InstallationServeurMongoDB.pdf)).
- Avoir installer le compilateur **Visual Studio 2019**.

### Les étapes pour l'IDE Visual Studio 2019


1. Décompresser le dossier "**MongoDBC.zip**" dans un dossier de votre choix.

![ExtractionFichierZIP.png](imgREADME%2FVISUALSTUDIO%2FExtractionFichierZIP.png)

2. Ouvrir le projet avec Visual Studio (dossier MongoDBC).

![OuvertureProjetVisualSTUDIO.png](imgREADME%2FVISUALSTUDIO%2FOuvertureProjetVisualSTUDIO.png)

À ce stade, vous devriez avoir les messages d'erreurs suivantes :

![ecranVISUALSTUDIO.png](imgREADME%2FVISUALSTUDIO%2FecranVISUALSTUDIO.png)

C'est normal, car par défaut Visual Studio 2019 choisit la version 64 bits pour compiler le projet.
Il faut donc changer la configuration du projet pour qu'il tourne en 32 bits.

3. Pour changer la configuration du projet, vous devez modifier la configuration qui est
à côté du bouton d'exécution du projet, vous devez faire défiler et cliquez sur "**Gérez les configurations..**".

![ConfigurationVersion.png](imgREADME%2FVISUALSTUDIO%2FConfigurationVersion.png)
4. Supprimez la configuration "**x64**" en la sélectionnant et en cliquant sur la croix rouge.

![version1.png](imgREADME%2FVISUALSTUDIO%2Fversion1.png)
5. Ajoutez une nouvelle configuration en cliquant sur la croix verte.

![AjoutConfiguration.png](imgREADME%2FVISUALSTUDIO%2FAjoutConfiguration.png)
6. Dans la fenêtre qui s'ouvre, sélectionnez "**x86-Debug**" et cliquez sur "**Sélectionner**", faite le même chose pour **x86-Release**.

7. Enfin executez la classe **main.c** en cliquant sur la flèche verte.

![resultat.png](imgREADME%2FVISUALSTUDIO%2Fresultat.png)


Une console s'ouvre et affiche le résultat de l'exécution du programme.

### Contenue de l'application

L'application contient 4 dossiers qui contiennent chacun un fichier :

- **CREATE** : Contient le fichier **create.c** qui contient toutes les fonctions qui permettent de simuler les requêtes de création de données dans la base de données.
- **UPDATE** : Contient le fichier **update.c** qui contient toutes les fonctions qui permettent de simuler les requêtes de mise à jour de données dans la base de données.
- **READ** : Contient le fichier **read.c** qui contient toutes les fonctions qui permettent de simuler les requêtes de lecture de données dans la base de données.
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

![captureFonctions.png](imgREADME%2FVISUALSTUDIO%2FcaptureFonctions.png)