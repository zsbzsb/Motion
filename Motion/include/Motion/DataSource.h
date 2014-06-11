#ifndef MOTION_DATASOURCE_H
#define MOTION_DATASOURCE_H

#include <Motion/Export.h>
#include <Motion/Types.h>

MOTION_C_API mtDataSource* mtDataSource_Create();
MOTION_C_API void mtDataSource_Destroy(mtDataSource* DataSource);

#endif
