/****************************************************************************
**
**  FougTools
**  Copyright Fougue (30 Mar. 2015)
**  contact@fougsys.fr
**
** This software is a computer program whose purpose is to provide utility
** tools for the C++ language, the Qt and Open Cascade toolkits.
**
** This software is governed by the CeCILL-C license under French law and
** abiding by the rules of distribution of free software.  You can  use,
** modify and/ or redistribute the software under the terms of the CeCILL-C
** license as circulated by CEA, CNRS and INRIA at the following URL
** "http://www.cecill.info".
**
** As a counterpart to the access to the source code and  rights to copy,
** modify and redistribute granted by the license, users are provided only
** with a limited warranty  and the software's author,  the holder of the
** economic rights,  and the successive licensors  have only  limited
** liability.
**
** In this respect, the user's attention is drawn to the risks associated
** with loading,  using,  modifying and/or developing or reproducing the
** software by the user in light of its specific status of free software,
** that may mean  that it is complicated to manipulate,  and  that  also
** therefore means  that it is reserved for developers  and  experienced
** professionals having in-depth computer knowledge. Users are therefore
** encouraged to load and test the software's suitability as regards their
** requirements in conditions enabling the security of their systems and/or
** data to be ensured and,  more generally, to use and operate it in the
** same conditions as regards security.
**
** The fact that you are presently reading this means that you have had
** knowledge of the CeCILL-C license and that you accept its terms.
**
****************************************************************************/

#pragma once

#include "occtools.h"
#include <Handle_Message_ProgressIndicator.hxx>
#include <Handle_StlMesh_Mesh.hxx>
#include <TopoDS_Shape.hxx>

namespace occ {

class OCCTOOLS_EXPORT IO
{
public:
    typedef const char* FileNameLocal8Bit;

    enum Format
    {
        IgesFormat,
        StepFormat,
        OccBrepFormat,
        AsciiStlFormat,
        BinaryStlFormat,
        UnknownFormat
    };

    static Format partFormat(FileNameLocal8Bit fileName);
    static Format partFormatFromContents(
            const char* contentsBegin,
            std::size_t contentsBeginSize,
            std::size_t fullContentsSizeHint = 0);

    static TopoDS_Shape loadPartFile(
            FileNameLocal8Bit fileName,
            Handle_Message_ProgressIndicator indicator = NULL);

    static Handle_StlMesh_Mesh loadStlFile(
            FileNameLocal8Bit fileName,
            Handle_Message_ProgressIndicator indicator = NULL);

    static TopoDS_Shape loadBrepFile(
            FileNameLocal8Bit fileName,
            Handle_Message_ProgressIndicator indicator = NULL);
    static TopoDS_Shape loadIgesFile(
            FileNameLocal8Bit fileName,
            Handle_Message_ProgressIndicator indicator = NULL);
    static TopoDS_Shape loadStepFile(
            FileNameLocal8Bit fileName,
            Handle_Message_ProgressIndicator indicator = NULL);

    static void writeBrepFile(
            const TopoDS_Shape& shape,
            FileNameLocal8Bit fileName,
            Handle_Message_ProgressIndicator indicator = NULL);
    static void writeIgesFile(
            const TopoDS_Shape& shape,
            FileNameLocal8Bit fileName,
            Handle_Message_ProgressIndicator indicator = NULL);
    static void writeStepFile(
            const TopoDS_Shape& shape,
            FileNameLocal8Bit fileName,
            Handle_Message_ProgressIndicator indicator = NULL);

    static void writeAsciiStlFile(
            const TopoDS_Shape& shape, FileNameLocal8Bit fileName);
    static void writeBinaryStlFile(
            const TopoDS_Shape& shape, FileNameLocal8Bit fileName);
};

} // namespace occ
