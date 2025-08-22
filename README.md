# Coding Challenge: Colisão Elástica Com Quadtree

![alt text](/src/assets/readme/preview.gif)

Este projeto visa simular um sistema de colisão elástica entre bolinhas 2D e entender a física e a matemática relacionada para conseguir resolver os problemas ao longo do processo de implementação. Para o desenvolvimento desse projeto foram usadas as seguintes tecnologias:

- C++
- CMake
- SFML

### O que é Colisão elástica?

Colisão elástica é um tipo de colisão onde os corpos não perdem energia cinética e não perdem momentum. Seus valores tanto antes da colisão quanto depois dela não sofrem alteração.

### Disclaimer e Arquivo Utilitário 

Além do código principal, onde está o coração da nossa aplicação, teremos um arquivo a mais chamado utility. Esse arquivo é responsável por guardar algumas funções úteis para nós mas que não necessariamente são imprescindíveis entrarem na explicação do processo de desenvolvimento. 
São funções, como gerar número aleatório considerando um valor mínimo e máximo e carregamento de fonte, além de uma classe para que possamos acompanhar o FPS em Tela. Não  irei passar por esse arquivo.

# Stress Test

Eu tenho um computador atualmente relativamente bom. Com meu computador atual, eu consigo renderizar 800 partículas simultanemante à 30 FPS. Algo que pode ser MUITO melhorado, muito mesmo, caso eu implementasse um outro algoritmo para validação das colisões, como as Quadtrees. Mas isso fica para próxima. :D

![alt text](/src/assets/readme/stress.gif)


## Fontes de Estudo
Conteúdo que foi valiosíssimo para o entendimento desse projeto:

- [Elastic Collision (Wikipedia)](https://en.wikipedia.org/wiki/Elastic_collision)
- [Dipam Sen – Collisions PDF](https://dipamsen.github.io/notebook/page/collisions.pdf)
- [SpicyYoghurt – Collision Physics](https://spicyyoghurt.com/tutorials/html5-javascript-game-development/collision-detection-physics)
- [HappyCoding – Collision Detection](https://happycoding.io/tutorials/processing/collision-detection)
- [Vídeo explicativo](https://www.youtube.com/watch?v=dJNFPv9Mj-Y&t=255s)

