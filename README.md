# trabalho_ed1
Trabalho de estrutura de dados 1 feito por Lucas e Leonardo.

---

 - imm -open file.txt 
 
	Abre uma imagem (formato texto) e mostra os valores dos pixels na tela 
  
 - imm -convert file.txt file.imm 
 
	Converte uma imagem no formato file.txt para o formato file.imm 
	
 - imm -open file.imm 
 
	Abra uma imagem (formato binária) e mostra os valores dos pixels na tela 
	
 - imm -segment thr file.imm segfile.imm 
 
	Faz o thresholding (limiarização da imagem) com um valor thr da imagem file.imm  e escreve o resultado em segfile.imm
	
 - imm -cc segfile.imm outfile.imm 
 
	Detecta os componentes conexos de uma imagem 
	
 - imm -lab imlab.txt imlabout.txt 
 
	Mostra o caminho a ser percorrido em um labirinto 
	
 - imm -outro-comando-que-não-existe
 
	Mostra uma mensagem de erro de comando não encontrado 