//
// Created by kony on 1.12.18.
//

#define LS  -1
#define GR   1
#define EQ   0
#define ER  -99

#define num_of_tokens 14

////////////////////////////////////////////// *    /   +   -   (   )   i   $   <   >  <=  >=  ==  !=
int SA_table[num_of_tokens][num_of_tokens] = {
/*                                    " * " */{ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/*                                    " / " */{ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/*                                    " + " */{ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/*                                    " - " */{ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/*                                    " ( " */{ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/*                                    " ) " */{ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/*                                    " i " */{ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/*                                    " $ " */{ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/*                                    " < " */{ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/*                                    " > " */{ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/*                                   " <= " */{ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/*                                   "  >=" */{ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/*                                   " == " */{ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},
/*                                   " != " */{ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER},};