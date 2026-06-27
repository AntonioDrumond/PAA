import sys
import argparse
import traceback

def executar_visualizador():
    try:
        import networkx as nx
        import matplotlib.pyplot as plt

        def extrair_grafos(linhas_texto):
            conjunto_grafos = {
                "Original": nx.DiGraph(),
                "Contraido": nx.DiGraph(),
                "Expandido": nx.DiGraph(),
                "Exponenciado": nx.DiGraph()
            }
            etapa_atual = "Original"
            nos_visitados = set()
            for linha_atual in linhas_texto:
                linha_atual = linha_atual.strip()
                if not linha_atual:
                    continue
                if 'CONTRACTED===================' in linha_atual:
                    etapa_atual = "Contraido"
                    nos_visitados = set()
                    continue
                elif 'EXPAND===================' in linha_atual:
                    etapa_atual = "Expandido"
                    nos_visitados = set()
                    continue
                elif 'EXPO===================' in linha_atual:
                    etapa_atual = "Exponenciado"
                    nos_visitados = set()
                    continue
                if linha_atual.startswith('Evaluating') or linha_atual.startswith('Edges removed'):
                    continue
                partes_linha = linha_atual.split()
                if len(partes_linha) == 1:
                    id_no = partes_linha[0]
                    if id_no not in nos_visitados:
                        nos_visitados.add(id_no)
                        conjunto_grafos[etapa_atual].add_node(id_no)
                elif len(partes_linha) == 2:
                    origem_aresta, destino_aresta = partes_linha[0], partes_linha[1]
                    conjunto_grafos[etapa_atual].add_edge(origem_aresta, destino_aresta)
            return conjunto_grafos

        def desenhar_grafos(grafos_extraidos):
            grafos_validos = [(nome_grafo, estrutura_grafo) for nome_grafo, estrutura_grafo in grafos_extraidos.items() if estrutura_grafo.number_of_nodes() > 0]
            if not grafos_validos:
                print("Nenhum grafo valido encontrado na entrada.")
                return
                
            quantidade_grafos = len(grafos_validos)
            colunas_grid = 2 if quantidade_grafos > 1 else 1
            linhas_grid = (quantidade_grafos + 1) // 2
            
            plt.style.use('dark_background')
            figura_principal, eixos_graficos = plt.subplots(linhas_grid, colunas_grid, figsize=(10 * colunas_grid, 8 * linhas_grid))
            figura_principal.patch.set_facecolor('#0f172a')
            
            if quantidade_grafos == 1:
                eixos_graficos = [eixos_graficos]
            elif linhas_grid > 1:
                eixos_graficos = eixos_graficos.flatten()
                
            figura_principal.canvas.manager.set_window_title('Visualizador de Grafos')
            
            for indice_atual, (nome_etapa, estrutura_atual) in enumerate(grafos_validos):
                eixo_atual = eixos_graficos[indice_atual]
                eixo_atual.set_facecolor('#0f172a')
                posicao_nos = nx.spring_layout(estrutura_atual, seed=42, k=0.9, iterations=100)
                nx.draw_networkx_nodes(estrutura_atual, posicao_nos, ax=eixo_atual, node_color='#3b82f6', 
                                       node_size=800, edgecolors='#93c5fd', linewidths=2, alpha=0.9)
                nx.draw_networkx_edges(estrutura_atual, posicao_nos, ax=eixo_atual, arrowstyle='-|>', arrowsize=22, 
                                       edge_color='#94a3b8', width=2, connectionstyle="arc3,rad=0.15", alpha=0.8)
                nx.draw_networkx_labels(estrutura_atual, posicao_nos, ax=eixo_atual, font_size=12, 
                                        font_family='sans-serif', font_weight='bold', font_color='white')
                titulo_eixo = f"Etapa: {nome_etapa} (Nos: {estrutura_atual.number_of_nodes()}, Arestas: {estrutura_atual.number_of_edges()})"
                eixo_atual.set_title(titulo_eixo, fontsize=16, fontweight='bold', color='#f8fafc', pad=15)
                eixo_atual.axis('off')
                
            for indice_vazio in range(indice_atual + 1, len(eixos_graficos)):
                eixos_graficos[indice_vazio].axis('off')
                
            plt.tight_layout(pad=4.0)
            plt.show()

        analisador_argumentos = argparse.ArgumentParser(description="Visualizar grafos da saida em C++.")
        analisador_argumentos.add_argument("arquivo", nargs="?", type=argparse.FileType("r"), default=sys.stdin)
        argumentos_recebidos = analisador_argumentos.parse_args()
        
        if argumentos_recebidos.arquivo.isatty():
            print("Aguardando entrada de dados pelo terminal...")
            
        linhas_lidas = argumentos_recebidos.arquivo.readlines()
        
        if not linhas_lidas:
            print("Nenhuma entrada fornecida.")
            return
            
        grafos_processados = extrair_grafos(linhas_lidas)
        desenhar_grafos(grafos_processados)
        
    except Exception as e:
        with open("error_log.txt", "w") as f:
            f.write("ERROR OCCURRED:\n")
            f.write(traceback.format_exc())
        raise e

if __name__ == "__main__":
    executar_visualizador()
