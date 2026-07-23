# lem-in

**Projet algorithmique élémentaire**

*Version : 2*

---

## Résumé

Ce projet a pour objectif de vous faire coder un gestionnaire de fourmilière. Vous recevrez un nombre défini de fourmis qu'il faudra faire passer d'un point de départ à une sortie, dans une fourmilière qui vous sera décrite dans un fichier. Votre but est de lire la description de la fourmilière, de trouver l'algorithme pour'acheminement des fourmis, puis d'afficher les instructions de déplacement de chaque fourmi à travers la fourmilière.

---

## Table des matières

- [I. Avant-propos](#i-avant-propos)
- [II. Introduction](#ii-introduction)
- [III. Objectifs](#iii-objectifs)
- [IV. Consignes générales](#iv-consignes-générales)
- [V. Partie obligatoire](#v-partie-obligatoire)
- [VI. Partie bonus](#vi-partie-bonus)
- [VII. Soumission et évaluation par les pairs](#vii-soumission-et-évaluation-par-les-pairs)

---

## I. Avant-propos

Voici un extrait de *Bill & John* :

```
Up to mighty London
Came an Irishman one day.
As the streets are paved with gold
Sure, everyone was gay,
Singing songs of Piccadilly,
Strand and Leicester Square,
Till Paddy got excited,
Then he shouted to them there:
It's a long way to Tipperary,
It's a long way to go.
It's a long way to Tipperary
To the sweetest girl I know!
Goodbye, Piccadilly,
Farewell, Leicester Square!
It's a long long way to Tipperary,
But my heart's right there.
(repeat)
```

Paddy a écrit une lettre à sa Molly irlandaise, disant : « Si tu ne la reçois pas, écris-moi pour me le faire savoir ! » « Si j'ai fait des fautes d'orthographe, Molly, ma chérie, » dit-il, « Souviens-toi, c'est le stylo qui est mauvais, ne me blâme pas ! »

Molly a écrit une belle réponse à son Paddy irlandais, disant : « Mike Maloney veut m'épouser, alors quitte le Strand et Piccadilly, ou tu en seras blâmé, car l'amour m'a rendue folle : en espérant que toi aussi ! »

---

## II. Introduction

La fourmilière sera définie par les liens suivants. Nous devons observer comment elles trouvent la sortie. Comment construisons-nous une fourmilière ? Nous avons besoin de tubes et de salles. Nous observons votre conception avant que le jeu ne commence. Au lieu de physically créer la fourmilière, créez une version numérique.

---

## III. Objectifs

La fourmilière est définie par un fichier au format suivant :

```
nombre_de_fourmis
les_salles
les_liens
```

Les salles sont composées de la manière suivante :

```
##start
1 23 3
2 16 7
#commentaire
3 16 3
4 16 5
5 9 3
6 1 5
7 4 8
##end
0 9 5
0-4
0-6
1-3
4-3
5-2
3-5
#autre_commentaire
4-2
2-1
7-6
7-2
7-4
6-5
#autre_commentaire
```

Votre programme doit afficher le chemin emprunté par chaque fourmi, avec le format suivant :

```
Lx-y Lz-w Lr-o ...
```

où x, z, r représentent les numéros des fourmis et y, w, o représentent les noms des salles.

---

## IV. Consignes générales

### Exigences

- Le fichier exécutable doit être nommé **lem-in**.
- Votre projet doit être écrit en **C**.
- Votre programme ne doit pas avoir de **fuites mémoire**.

### Fonctions autorisées

Seules les fonctions suivantes sont autorisées :

- `malloc`
- `free`
- `read`
- `write`
- `strerror`
- `perror`
- `exit`

L'utilisation de toute autre fonction n'est pas autorisée sauf si son utilisation est justifiée lors de la défense.

### Ressources

- Vous pouvez poser des questions sur le forum et Slack.

---

## V. Partie obligatoire

### Sujet

Créez votre gestionnaire de fourmilière. Votre programme ne doit gérer que les situations valides contenues dans le fichier d'entrée. Vous devez produire la sortie au format demandé ci-dessous.

Votre programme recevra une description de fourmilière décrivant les salles et les tunnels. Chaque salle possède des coordonnées (x, y). Chaque tunnel relie deux salles et ne peut être emprunté que par une seule fourmi à la fois (l'extrémité réceptrice doit être vide).

### Format d'entrée

```
nombre_de_fourmis
les_salles
les_liens
```

### Format de sortie

```
L1-2
L1-3 L2-2
L1-1 L2-3 L3-2
L2-1 L3-3
L3-1
```

où L1, L2, L3 représentent les numéros des fourmis et 1, 2, 3 représentent les noms des salles.

### Exemples

#### Exemple 1

```
[0]-[2]-[3]-[1]
```

Entrée :
```
3
##start
0 1 0
##end
1 5 0
2 9 0
3 13 0
0-2
2-3
3-1
```

Sortie :
```
L1-2
L1-3 L2-2
L1-1 L2-3 L3-2
L2-1 L3-3
L3-1
```

#### Exemple 2.1

```
[0]
/    \
[1]      [2]
 |        |
 |       [3]
  \    /
    [4]
```

Entrée :
```
2
1 0 2
##start
0 2 0
##end
4 2 6
2 4 2
3 4 4
0-1
0-2
2-3
3-4
4-1
```

Sortie :
```
L1-1
L1-4 L2-1
L2-4
```

#### Exemple 2.2

```
3
1 0 2
##start
0 2 0
##end
4 2 6
2 4 2
3 4 4
0-1
0-2
2-3
3-4
4-1

L1-1 L2-2
L1-4 L3-1 L2-3
L3-4 L2-4
```

#### Exemple 3

```
[start]
 / |
[3] [1]--[5]
/    |    |
[4]--[2] [6]
      | /
     [end]
```

Entrée :
```
4
3 2 2
##start
start 4 0
##end
end 4 6
4 0 4
1 4 2
2 4 4
5 8 2
6 8 4
start-1
3-4
2-4
1-5
6-5
end-6
1-2
2-end
3-start
```

> **Note** : Ceci n'est pas aussi simple que cela semble.

---

## VI. Partie bonus

Vous pouvez créer des fonctionnalités supplémentaires selon vos propres goûts.

### Idées de bonus

- Pourquoi ne pas coder un **visualiseur de fourmilière** ?
- Ajouter des options visuelles (telles que diverses couleurs ou niveaux).

---

## VII. Soumission et évaluation par les pairs

Soumettez votre travail selon les procédures habituelles de l'école. Assurez-vous de vérifier attentivement vos dossiers et fichiers pour vous assurer qu'ils sont corrects.

---

*Équipe pédagogique : pedago@42.fr*
