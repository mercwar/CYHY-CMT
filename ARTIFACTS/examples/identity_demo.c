#include "../include/avis_identity.h"

int main()
{
    AVIS_ArtifactIdentity identity;

    avis_identity_init(&identity);

    strcpy(identity.file.file_name,"AVIS-FVS-SNAPSHOT.c");

    strcpy(identity.disk.absolute_path,
           "B:\\AVIS\\ENGINE\\FVS\\AVIS-FVS-SNAPSHOT.c");

    avis_identity_print(&identity);

    return 0;
}
