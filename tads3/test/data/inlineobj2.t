#include "tads.h"

main(args)
{
	local s = 'String!!!\n';
    local o = object {
        name = 'a name';
        action() {
			"name=<<name>>\n";
            "<<s>>";
        }
    };

	o.action();
}
