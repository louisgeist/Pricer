# Option's Pricer
## Idées générales
### But du programme
Dans le cadre du modèle de Black-Scholes :
- calcul du prix d'options :
  - européennes sur sous-jacent avec ou sans dividendes (utilisation des formules explicites du modèle de *Black-Scholes-Merton*),
  - américaines (par la méthode de *Longstaff & Schwartz*),
  - asiatiques (par méthode de *Monte-Carlo*),
- affichage de la stratégie de réplication d'options (européennes sur sous-jacent avec ou sans dividendes - utilisation de la parité call-put)

### Conventions et hypothèses
Quelques hypothèses importantes optées pour notre programme ;
- *r* : le taux sans risque est constant,
- aucun affichage du programme ne présente une unité pour les prix : le programme donne de bons résultats tant que tous les prix sont utilisés avec la même devise.

## Architecture du programme
### Vue d'ensemble de l'architecture: Diagramme de classes UML
Choix de représentation :
- les Getter et Setter pour chaque attribut des classes sont bien définis dans le code, nous ne les écrivons pas dans le diagramme de classe. 
- Pareil pour le constructeur par défaut, par copie et le destructeur

![DiagramUML.png]

Légende du diagramme de classe UML:
- La flèche indique un héritage. La classe mère de l'héritage est pointée par la flèche.
- Les traits avec un losange indique :
  - pour un losange noir : une relation de composition (le losange est de l'agrégat)
  - pour un losange creux : une relation d'agrégation (le losange ets du côté de l'objet qui est ag)

### Classe asset
Nous avons une classe **asset** pour les actifs financiers.
Les variables membres de la classe *asset* sont :
- AssetName : un pointeur vers une chaîne de caractères qui désigne le nom de l'actif,
- CurrentTime : la date à laquelle est évaluée l'actif,
- SpotPrice : le prix spot de l'actif,
- Volatility : la volatilité de l'action,
- Dividends : les dividendes futures de l'actif, qui est un objet de la classe dividend.

La classe **dividend** est une classe agrégée à la classe *asset*.
Ses variables membres sont :
- Type : un entier dans $\{0,1,2\}$ qui indique le type de dividendes :
  - $0$ : signifie qu'il n'y a pas de dividendes,
  - $1$ : désigne des dividendes à date de paiments discrets,
  - $2$ : désigne des dividendes à paiements continus.
- Rate : le taux des dévidendes
- Periods : la période entre deux paiments (pertinent que pour une dividende de type 1)
- Next : durée jusqu'au prochain paiment (pertinent que pour une dividende de type 1)


### Classe option
La classe *option* est une classe abstraite. Ses attributs sont :
- K : le *strike* de l'option,
- sigma : la *volatilité* du sous-jacent,
- ptr_underlying : pointeur vers un objet de la classe *asset*.

Nous avons définis trois paires de classes qui héritent de cette interface *option* :
- **Options européennes** : 
  - une classe *european_call*, 
  - une classe *european_put*,
- **Options américaines** : 
  - une classe *american_call*,
  - une classe *american_put*,
- **Options asiatiques** : 
  - une classe *asian_call*,
  - une classe *asian_put*.

## Fonctionnalités du programme
### Fonctions
Les fonctions suivantes ont été implémentées pour toutes les classes du programme.

#### Constructeurs et destructeurs
Les constructeurs :
- default constructor,
- copy constructor,
- constructor with arguments.

#### Fonctions membres
Les fonctions membres classiques :
- les *Getters*,
- les *Setters*,

#### Fonctions non-membres
Nous avons surchargé les opérateurs de iostream ">>" et "<<" pour toutes qui héritent de la classe *option* et les *asset*.

### Fonctions spécifiques aux classes
#### Classe asset
- asset estimation
- asset actulalisation

**JR**

#### Classe option
Les fonctions membres spécifiques sont toutes des fonction purement virtuelles, qui  :
- type() :  qui retourne (en string) le type de l'option (european put, asian call, etc...  : c'est-à-dire le nom de la classe fille)
- price() : qui retourne le prix actuel de l'option ainsi définie.
- replication() : qui affiche comment répliquer l'option (Nous ne savons pas répliquer des options autres qu'européennes, donc nous avons renvoyé un message d'avertissement pour les autres options.)


## Critique des problèmes rencontrés et des solutions adoptées
### Organisation d'un programme de grande ambition
La classe option (et ce qui en hérite), d'un côté, et la classe asset (et la classe dividend ), d'un autre, ont été codé par deux personnes. Nous avions réalisé des rendez-vous réguliers pour se mettre d'accord comment coder et aborder le programme, mais lorsqu'il était venu d'utiliser la classe asset dans la classe option (afin de valoriser des options sur sous-jacents avec dividendes), nous nous sommes rendus comptes de petites spécificités à nos deux manières de coder que nous n'avions pas anticipées. 

Fort de cette expérience, nous en concluons que nous devrons pour un prochain projet de programmation, davantage nous mettre d'accord sur les fonctionnalités de chacune des classes, notamment sur les types des arguments et le type renvoyé des fonctions. Une idée aussi serait d'essayait de réaliser un diagramme de classes avant le début du codage en ajoutant les appels de fonctions entre classes qui seront mobilisés.

### Modification d'un attribut d'une classe attribut
**JR**
Problème :

Solution : création de la fonction membre de asset `get_alias_dividend` un "getter" non standard. Le getter n'est pas de type `const`. Cela permet alors depuis l'asset accéder à dividend.

# Méthodologie de pricing
## Les notations
On note de manière classique dans le cadre d'un modèle Black-Scholes :
- K : le *strike* de l'option,
- S : le *spot price* du sous-jacent,
- r : le taux sans risque,
- sigma : la volatilité du sous-jacent,
- T : la maturité de l'option.

Pour les options avec dividendes :
- On note le prix d'une option call européenne de maturité $T$ et de strike $K$ sur un sous-jacent de volatilité $\sigma$ et de prix spot $S$ : $c(S,\sigma,T,K)$,
- $\delta$ : le taux de dividendes.
- $n$ : [pour des lump payments] nombre de détachements de dividendes durant la détention de l'option

## Formules utilisées
### European options
- European sans dividendes
  - call : formule de Black-Scholes : $c=SN(d_{1})-e^{-rT}N(d_{2})$
    - où : $N(.)$ est la fonction de répartition de $\mathcal{N}(0,1)$
  - put : call-put parity : $c-p=S_{T}-e^{-rT}K$

- European continuous dividend :
  - call : $c_{continuous}=c(e^{-\delta T}S_{0},\sigma,T,K)$
  - put : $c_{continuous}-p_{continuous}=e^{-\delta T}S_{0}-e^{-rT}K$
  - Remarque : dans le code, j'ai nommé S_hat $=e^{-\delta T}S_{0}$

- European lump payment dividend : 
  - call : $c_{lump\; payments}=c((1-\delta)^{n}S_{0},\sigma,T,K)$
  - put : $c_{lump\;payments}-p_{lump\; payments}=(1-\delta)^{n}S_{0}-e^{-rT}K$
  - Remarque : dans le code, j'ai nommé S_hat $=e^{-\delta T}S_{0}$


### Asian options
On note $\bar{S}= \frac{1}{T}\int\limits_{0}^{T}S_{t}dt$
  - call : $V_{T}=(\bar{S}-K)^{+}$
  - put : $V_{T}=(\bar{S}-K)^{-}$


### American
**JR**
