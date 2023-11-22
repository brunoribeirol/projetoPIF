<h1 align="center">C->RUN</h1>

<p align="center">
  <img
    src="https://img.shields.io/github/repo-size/brunoribeirol/projetoPIF?style=flat"
    alt="Repository Size"
  />
  <img
    src="https://img.shields.io/github/commit-activity/t/brunoribeirol/projetoPIF?style=flat&logo=github"
    alt="Commit Activity"
  />
  <a href="LICENSE.md"
    ><img
      src="https://img.shields.io/github/license/brunoribeirol/projetoPIF?style"
      alt="License"
  /></a>
</p>

<p>O C->RUN Game é um jogo simples desenvolvido em C, onde o jogador controla uma bola que se move para a esquerda ou direita para desviar de obstáculos em queda. O objetivo do jogo é acumular a maior pontuação possível, desviando dos obstáculos que descem pela tela.</p>

<h2>Funcionalidades</h2>
<ul>
    <li><strong>Jogar C->RUN:</strong> Inicie o jogo e controle a bola usando as teclas 'a' para mover para a esquerda e 'd' para mover para a direita.</li>
    <li><strong>Visualizar Ranking:</strong> Veja o ranking dos jogadores com base na pontuação alcançada.</li>
    <li><strong>Sair:</strong> Encerre o programa.</li>
</ul>

<h2>Instruções de Compilação e Execução:</h2>
<pre>
    <code>
        make
    </code>
</pre>

<h2>Regras do Jogo</h2>
<ul>
    <li>A bola se move automaticamente para a esquerda ou direita, dependendo das teclas 'a' e 'd' pressionadas pelo jogador.</li>
    <li>Objetivo: Desviar dos obstáculos que descem pela tela.</li>
    <li>O jogo termina quando a bola colide com um obstáculo.</li>
    <li>A pontuação é baseada na quantidade de obstáculos desviados com sucesso.</li>
</ul>

<h2>Arquivos de Pontuação</h2>
<p>As pontuações dos jogadores são salvas no arquivo <code>scores.txt</code>. Cada linha do arquivo contém o nome do jogador e a pontuação correspondente.</p>

<h2>Equipe</h2>
<ul>
  <li>
    <a href="https://github.com/brunoribeirol">Bruno Ribeiro</a> -
    brlla@cesar.school 📩
  </li>
  <li>
    <a href="https://github.com/Victorgalves">Victor Guilherme</a> -
    jprda@cesar.school 📩
  </li>
  <li>
    <a href="https://github.com/vinipetribu">Vinícius Petribu</a> -
    vclp@cesar.school 📩
  </li>
</ul>

<h2>Contribuíntes 👨‍👩‍👧‍👦</h2>
<a href="https://github.com/brunoribeirol/projetoPIF/graphs/contributors">
  <img src="https://contrib.rocks/image?repo=brunoribeirol/projetoPIF" />
</a>

<h2>Notas Adicionais</h2>
<ul>
    <li>O código utiliza a biblioteca padrão <code>stdio.h</code>, <code>stdlib.h</code>, <code>string.h</code>, <code>unistd.h</code>, e as bibliotecas customizadas <code>screen.h</code>, <code>keyboard.h</code>, <code>timer.h</code>.</li>
    <li>A pontuação é salva no arquivo <code>scores.txt</code> após cada jogo.</li>
    <li>Este projeto foi desenvolvido como um exercício simples para prática em programação na linguagem C.</li>
    <li>Qualquer contribuição ou sugestão é bem-vinda.</li>
</ul>
<p>Divirta-se jogando C-RUN!</p>
</body>
</html>



