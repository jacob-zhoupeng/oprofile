/**
 * @file sample_container.cpp
 * Internal container for samples
 *
 * @remark Copyright 2002, 2003 OProfile authors
 * @remark Read the file COPYING
 *
 * @author Philippe Elie
 * @author John Levon
 */

#include <set>
#include <numeric>
#include <algorithm>
#include <vector>

#include "opp_symbol.h"
#include "symbol_functors.h"
#include "sample_container.h"

using namespace std;

namespace {

inline unsigned int add_counts(unsigned int count, sample_entry const * s)
{
	return count + s->count;
}

} // namespace anon


sample_container::samples_iterator sample_container::begin() const
{
	return samples.begin();
}


sample_container::samples_iterator sample_container::end() const
{
	return samples.end();
}


sample_container::samples_iterator
sample_container::begin(symbol_entry const * symbol) const
{
	samples_storage::key_type key(symbol, 0);

	return samples.lower_bound(key);
}


sample_container::samples_iterator 
sample_container::end(symbol_entry const * symbol) const
{
	samples_storage::key_type key(symbol, ~bfd_vma(0));

	return samples.upper_bound(key);
}


void sample_container::insert(symbol_entry const * symbol,
                              sample_entry const & sample)
{
	samples_storage::key_type key(symbol, sample.vma);

	samples_storage::iterator it = samples.find(key);
	if (it != samples.end()) {
		it->second.count += sample.count;
	} else {
		samples[key] = sample;
	}
}


unsigned int
sample_container::accumulate_samples(string const & filename) const
{
	build_by_loc();

	sample_entry lower, upper;

	debug_name_id const id = debug_names.create(filename);

	lower.file_loc.filename = upper.file_loc.filename = id;
	lower.file_loc.linenr = 0;
	upper.file_loc.linenr = INT_MAX;

	typedef samples_by_loc_t::const_iterator iterator;

	iterator it1 = samples_by_loc.lower_bound(&lower);
	iterator it2 = samples_by_loc.upper_bound(&upper);

	return accumulate(it1, it2, 0, add_counts);
}


sample_entry const *
sample_container::find_by_vma(symbol_entry const * symbol, bfd_vma vma) const
{
	sample_index_t key(symbol, vma);
	samples_iterator it = samples.find(key);
	if (it != samples.end())
		return &it->second;

	return 0;
}


unsigned int
sample_container::accumulate_samples(string const & filename,
                                     size_t linenr) const
{
	build_by_loc();

	sample_entry sample;

	sample.file_loc.filename = debug_names.create(filename);
	sample.file_loc.linenr = linenr;

	typedef pair<samples_by_loc_t::const_iterator,
		samples_by_loc_t::const_iterator> it_pair;

	it_pair itp = samples_by_loc.equal_range(&sample);

	return accumulate(itp.first, itp.second, 0, add_counts);
}


void sample_container::build_by_loc() const
{
	if (!samples_by_loc.empty())
		return;

	samples_iterator cit = samples.begin();
	samples_iterator end = samples.end();
	for (; cit != end; ++cit)
		samples_by_loc.insert(&cit->second);
}
