#include <wex.h>
#include "cGUI.h"
#include "GraphTheory.h"

#include "shunter.h"

cGUI::cGUI()
    : cStarterGUI(
          "Shunter",
          {50, 50, 1000, 500})
{
    fm.events().draw(
        [&](PAINTSTRUCT &ps)
        {
            wex::shapes S(ps);
            displayTrainPaths(S);
        });

    show();
    run();
}

void cGUI::displayTrainPaths(wex::shapes &S)
{
    int row = 0;

    // loop over trains in the order that they are to be moved
    for (int train : gPI.order)
    {
        S.text("train" + std::to_string(train) + ":",
               {20, 50 + 50 * row, 50, 25});
        std::string line;
        for (int station : gPI.paths[train])
        {
            line += gPI.gd.g.userName(station) + " ";
        }
        S.text(line, {80, 50 + 50 * row, 200, 25});
        row++;
    }

    // display any blocked trains
    for (int train = 0; train < gPI.init_positions.size(); train++)
        if (!gPI.paths[train].size())
        {
            S.text("train" + std::to_string(train) + ": blocked",
                   {20, 50 + 50 * row, 200, 25});
            row++;
        }
}
