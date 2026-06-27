import sys
import argparse
import networkx as nx
import matplotlib.pyplot as plt

def parse_input(lines):
    graphs = {
        "Original": nx.DiGraph(),
        "Contracted": nx.DiGraph(),
        "Expand": nx.DiGraph(),
        "Expo": nx.DiGraph()
    }
    
    current_phase = "Original"
    seen_nodes = set()
    
    for line in lines:
        line = line.strip()
        if not line:
            continue
            
        if 'CONTRACTED===================' in line:
            current_phase = "Contracted"
            seen_nodes = set()
            continue
        elif 'EXPAND===================' in line:
            current_phase = "Expand"
            seen_nodes = set()
            continue
        elif 'EXPO===================' in line:
            current_phase = "Expo"
            seen_nodes = set()
            continue
            
        if line.startswith('Evaluating') or line.startswith('Edges removed'):
            continue
            
        parts = line.split()
        
        if len(parts) == 1:
            node_id = parts[0]
            if node_id not in seen_nodes:
                seen_nodes.add(node_id)
                graphs[current_phase].add_node(node_id)
        elif len(parts) == 2:
            u, v = parts[0], parts[1]
            graphs[current_phase].add_edge(u, v)
            
    return graphs

def visualize_graphs(graphs):
    # Plot each graph in a subplot if it has nodes
    valid_graphs = [(name, g) for name, g in graphs.items() if g.number_of_nodes() > 0]
    
    if not valid_graphs:
        print("No valid graphs found in the input.")
        return
        
    num_graphs = len(valid_graphs)
    
    # Calculate grid size for subplots
    cols = 2 if num_graphs > 1 else 1
    rows = (num_graphs + 1) // 2
    
    fig, axes = plt.subplots(rows, cols, figsize=(7 * cols, 6 * rows))
    if num_graphs == 1:
        axes = [axes]
    elif rows > 1:
        axes = axes.flatten()
        
    fig.canvas.manager.set_window_title('Graph Visualizer')
    
    for i, (name, g) in enumerate(valid_graphs):
        ax = axes[i]
        
        # Use a nice layout
        pos = nx.spring_layout(g, seed=42)
        
        # Draw nodes
        nx.draw_networkx_nodes(g, pos, ax=ax, node_color='skyblue', node_size=600, 
                               edgecolors='black', linewidths=1.5)
        # Draw edges
        nx.draw_networkx_edges(g, pos, ax=ax, arrowstyle='->', arrowsize=20, 
                               edge_color='gray', width=1.5, connectionstyle="arc3,rad=0.1")
        # Draw labels
        nx.draw_networkx_labels(g, pos, ax=ax, font_size=10, font_family='sans-serif', font_weight='bold')
        
        ax.set_title(f"Stage: {name} (Nodes: {g.number_of_nodes()}, Edges: {g.number_of_edges()})", fontsize=14, fontweight='bold')
        ax.axis('off')
        
    # Hide empty subplots
    for j in range(i + 1, len(axes)):
        axes[j].axis('off')
        
    plt.tight_layout()
    plt.show()

def main():
    parser = argparse.ArgumentParser(description="Visualize graphs from C++ output.")
    parser.add_argument("file", nargs="?", type=argparse.FileType("r"), default=sys.stdin,
                        help="Input file containing graph stages. Reads from stdin if not provided.")
    args = parser.parse_args()
    
    if args.file.isatty():
        print("Waiting for input from stdin... (Press Ctrl+D or Ctrl+Z to finish)")
        
    lines = args.file.readlines()
    
    if not lines:
        print("No input provided. Please pipe output or provide a file.")
        return
        
    graphs = parse_input(lines)
    visualize_graphs(graphs)

if __name__ == "__main__":
    main()
