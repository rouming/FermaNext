
#include "Config.h"
#include <iostream>

int main ()
{
    Config& cfg = Config::instance("../build/config.xml");
    ConfigNode root = cfg.rootNode();
    std::cout << qPrintable( root.getTagName() ) << "\n";

    ConfigNodeList childs = root.childNodes();
    foreach ( Config::Node node, childs ) {
        std::cout << "\t" << qPrintable( node.getTagName() ) << "\n";
        node.addAttribute( NodeAttribute("FF", "GGG") );
        node.addAttribute( NodeAttribute("FF3333", "GGG") );

        node.remove();
    }

    ConfigNode node = root.createChildNode("TrussUnit");
    node.addAttribute( NodeAttribute("FF", "GGG") );
    node = node.createChildNode("Color");
    node.addAttribute( NodeAttribute("COlor", "GGG") );

    return 0;
}

