/* Generates micromagnetic XML. */

#include "template.h"

int main(int argc, char** argv)
{
    return template_files("fragments/micromagnetics_template.xml",
                          "output.xml");
}
