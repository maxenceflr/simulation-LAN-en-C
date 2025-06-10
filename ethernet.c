#include <ethernet.h>

void afficher_tram_user(const Trame *trame)
{
    if (trame == NULL)
    {
        fprintf(stderr, "Erreur de lecture de la trame\n");
        return;
    }

    printf("Preambule: ");
    for (int i = 0; i < 7; i++)
    {
        printf("%02x ", trame->preambule[i]);
    }
    printf("\n");

    printf("SFD: %02x\n", trame->SFD);

    printf("Adresse Destinataire: ");
    for (int i = 0; i < 6; i++)
    {
        printf("%02x ", trame->adrDest.bytes[i]);
    }
    printf("\n");

    printf("Adresse Source: ");
    for (int i = 0; i < 6; i++)
    {
        printf("%02x ", trame->adrSrc.bytes[i]);
    }
    printf("\n");

    printf("Type: %02x\n", trame->type);

    if (trame->data != NULL)
    {
        printf("Data: ");
        for (size_t i = 0; i < sizeof(trame->data); i++)
        {
            printf("%02x ", trame->data[i]);
        }
        printf("\n");
    }
    else
    {
        printf("Data: NULL\n");
    }

    printf("FCS: %08x\n", trame->FCS);
}