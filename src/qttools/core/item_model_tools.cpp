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

#include "qttools/core/item_model_tools.h"

#include <QtCore/QRegExp>
#include "qttools/core/qobject_tools.h"

namespace qttools {

bool isValidRow(const QAbstractItemModel* model,
                int row, const QModelIndex& parent)
{
  if (model == 0)
    return false;
  return 0 <= row && row < model->rowCount(parent);
}

bool isValidColumn(const QAbstractItemModel* model,
                   int col, const QModelIndex& parent)
{
  if (model == 0)
    return false;
  return 0 <= col && col < model->columnCount(parent);
}

/*! \brief Try to find a value in a given column of a model
 *  \return Index of the row where the first match of \p value could be found
 *  \retval -1 if \p value could not be found
 */
int findDataInRow(const QAbstractItemModel* model,
                  int col, const QVariant& value)
{
  for (int iRow = 0; iRow < model->rowCount(); ++iRow)
    if (model->data(model->index(iRow, col)) == value)
      return iRow;
  return -1;
}

/*! \brief Same as QAbstractItemModel::data() but more concise
 */
QVariant tableModelData(const QAbstractItemModel* model,
                        int row, int col, int role)
{
  return model->data(model->index(row, col), role);
}

bool isRowScheduledForDeletion(const QAbstractItemModel* model, int row)
{
  if (model == 0 || row >= model->rowCount())
    return false;
  const QVariant rowHeaderDisplayData =
      model->headerData(row, Qt::Vertical, Qt::DisplayRole);
  const QRegExp remRowRx("\\s*\\!\\s*");
  return remRowRx.indexIn(rowHeaderDisplayData.toString().simplified()) != -1;
}

} // namespace qttools