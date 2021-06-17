// considerando que a borda da imagem são zeros
// im - imagem original
// im_rot - imagem rotulada - inicialmente zeradalabel = 1;
lista_proximos = cria_lista();
Ponto p, p_atual;
    
for i = 1 ate nlinhas - 1 {
    for j = 1 ate ncolunas - 1 {
        // percorre toda a imagem em busca de um pixel foreground (valor 1)
        p.x = i;
        p.y = j;
        if (im(p.x, p.y) == 1) and(im_rot(p.x, p.y) == 0) {
            // atribui o label a posição (i,j)
            im_rot(p.x, p.y) = label;
            // inclui na lista de busca dos vizinhos
            lista_proximos.push_back(p);
            
            while !vazia(lista_proximos) {
                // busca o próximo ponto da lista
                p_atual = pop(lista_proximos);

                for(int i = 1; i < 5; i++) {
                    switch (i) {
                        // buscando por pixels na vizinhança do ponto atual que são iguais a 1
                        case 1:
                            // ponto acima 
                            p.x = p_atual.x - 1;
                            p.y = p_atual.y;
                            break;

                        case 2:
                            // ponto abaixo
                            p.x = p_atual.x + 1;
                            p.y = p_atual.y;
                            break;

                        case 3:
                            // ponto à esquerda
                            p.x = p_atual.x;
                            p.y = p_atual.y - 1;
                            break;    

                        case 4:
                            // ponto à direita
                            p.x = p_atual.x;
                            p.y = p_atual.y + 1;
                            break;    
                        
                        default:
                            break;
                    }

                    if (im(p.x, p.y) == 1) and(im_rot(p.x, p.y) == 0) {
                        // atribui o label a posição atual
                        im_rot(p.x, p.y) = label;
                        // adiciona o ponto na lista para verificar vizinhos posteriormente
                        push(lista_proximos, p);
                    }
                }
            } // enquanto
            label++;
        } // if
    }
}