#include "tads.h"

main(args)
{
    local o = object {
        name = 'a name';
        action() {
			"name=<<name>>\n";
            "String...\n";
        }
    };

	o.action();
}

