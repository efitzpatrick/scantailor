/*
    Scan Tailor - Interactive post-processing tool for scanned pages.
    Copyright (C) 2007-2008  Joseph Artsimovich <joseph_a@mail.ru>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef PAGE_LAYOUT_SETTINGS_H_
#define PAGE_LAYOUT_SETTINGS_H_

#include "NonCopyable.h"
#include "RefCountable.h"
#include <memory>

class PageId;
class Margins;
class QSizeF;

namespace page_layout
{

class Params;
class Alignment;

class Settings : public RefCountable
{
	DECLARE_NON_COPYABLE(Settings)
public:
	enum AggregateSizeChanged { AGGREGATE_SIZE_UNCHANGED, AGGREGATE_SIZE_CHANGED };
	
	Settings();
	
	virtual ~Settings();
	
	/**
	 * \brief Removes all stored data.
	 */
	void clear();
	
	/**
	 * \brief Get all page parameters at once.
	 *
	 * May return a null auto_ptr if the specified page is unknown to us.
	 */
	std::auto_ptr<Params> getPageParams(PageId const& page_id) const;
	
	/**
	 * \brief Set all page parameters at once.
	 */
	void setPageParams(PageId const& page_id, Params const& params);
	
	/**
	 * \brief Updates content size and returns all parameters at once.
	 */
	Params updateContentSizeAndGetParams(
		PageId const& page_id, QSizeF const& content_size_mm);
	
	/**
	 * \brief Returns the hard margins for the specified page.
	 *
	 * Hard margins are margins that will be there no matter what.
	 * Soft margins are those added to extend the page to match its
	 * size with other pages.
	 * \par
	 * If no margins were assigned to the specified page, the default
	 * margins are returned.
	 */
	Margins getHardMarginsMM(PageId const& page_id) const;
	
	/**
	 * \brief Sets hard margins for the specified page.
	 *
	 * Hard margins are margins that will be there no matter what.
	 * Soft margins are those added to extend the page to match its
	 * size with other pages.
	 */
	void setHardMarginsMM(PageId const& page_id, Margins const& margins_mm);
	
	/**
	 * \brief Returns the alignment for the specified page.
	 *
	 * Alignments affect the distribution of soft margins.
	 * \par
	 * If no alignment was specified, the default alignment is returned,
	 * which is "center vertically and horizontally".
	 */
	Alignment getPageAlignment(PageId const& page_id) const;
	
	/**
	 * \brief Sets alignment for the specified page.
	 *
	 * Alignments affect the distribution of soft margins.
	 */
	void setPageAlignment(PageId const& page_id, Alignment const& alignment);
	
	/**
	 * \brief Sets content size in millimeters for the specified page.
	 *
	 * The content size comes from the "Select Content" filter.
	 */
	AggregateSizeChanged setContentSizeMM(
		PageId const& page_id, QSizeF const& content_size_mm);
	
	/**
	 * \brief Returns the aggregate (max width + max height) hard page size.
	 */
	QSizeF getAggregateHardSizeMM() const;
	
	/**
	 * \brief Same as getAggregateHardSizeMM(), but assumes a specified
	 *        size for a specified page.
	 *
	 * This function doesn't modify anything, it just pretents that
	 * the size of a specified page has changed.
	 */
	QSizeF getAggregateHardSizeMM(
		PageId const& page_id, QSizeF const& hard_size_mm) const;
	
	/**
	 * \brief Find the widest page and return its id.
	 *
	 * We only consider the hard page size, that is content size + hard
	 * margins.
	 * \return The page id of the widest page or a null PageId if no
	 *         pages were defined.
	 */
	PageId findWidestPage() const;
	
	/**
	 * \brief Find the tallest page and return its id.
	 *
	 * We only consider the hard page size, that is content size + hard
	 * margins.
	 * \return The page id of the tallest page or a null PageId if no
	 *         pages were defined.
	 */
	PageId findTallestPage() const;
private:
	class Impl;
	class Item;
	class ModifyMargins;
	class ModifyAlignment;
	class ModifyContentSize;
	
	std::auto_ptr<Impl> m_ptrImpl;
};

} // namespace page_layout

#endif