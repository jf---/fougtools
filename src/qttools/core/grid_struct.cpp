/****************************************************************************
**
**  FougTools
**  Copyright FougSys (1 Mar. 2011)
**  contact@fougsys.fr
**
** This software is a computer program whose purpose is to provide utility
** tools for the C++ language and the Qt toolkit.
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

#include "grid_struct.h"

namespace qttools {

/*!
 * \class GridStruct
 * \brief Basic data for a grid structure : row and column count + cell numbering
 * \headerfile grid_struct.h <qttools/core/grid_struct.h>
 * \ingroup qttools_core
 *
 */

//! Constructs an invalid grid having no items, ie. itemCount() == 0
GridStruct::GridStruct()
    : m_rowCount(0),
      m_colCount(0)
{
}

GridStruct::GridStruct(int rows, int cols)
    : m_rowCount(rows),
      m_colCount(cols)
{
}

bool GridStruct::isValid() const
{
    return this->itemCount() > 0;
}

int GridStruct::itemCount() const
{
    return this->rowCount() * this->columnCount();
}

void GridStruct::setItemCount(int rows, int cols)
{
    this->setRowCount(rows);
    this->setColumnCount(cols);
}

int GridStruct::rowCount() const
{
    return m_rowCount;
}

void GridStruct::setRowCount(int count)
{
    m_rowCount = count;
}

int GridStruct::columnCount() const
{
    return m_colCount;
}

void GridStruct::setColumnCount(int count)
{
    m_colCount = count;
}

const qttools::GridNumbering& GridStruct::itemNumbering() const
{
    return m_gridNb;
}

void GridStruct::setItemNumbering(const qttools::GridNumbering& gridNb)
{
    m_gridNb = gridNb;
}

} // namespace qttools
