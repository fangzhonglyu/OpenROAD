///////////////////////////////////////////////////////////////////////////
//
// BSD 3-Clause License
//
// Copyright (c) 2022, The Regents of the University of California
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.
//
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// * Neither the name of the copyright holder nor the names of its
//   contributors may be used to endorse or promote products derived from
//   this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//
///////////////////////////////////////////////////////////////////////////////
#include "Hypergraph.h"
#include "Refiner.h"
#include "Utilities.h"
#include "utl/Logger.h"

// ------------------------------------------------------------------------------
// K-way hyperedge greedy refinement
// ------------------------------------------------------------------------------

namespace par {

GreedyRefine::GreedyRefine(const int num_parts,
                           const int refiner_iters,
                           const float path_wt_factor,
                           const float snaking_wt_factor,
                           const int max_move,
                           EvaluatorPtr evaluator,
                           utl::Logger* logger)
    : Refiner(num_parts,
              refiner_iters,
              path_wt_factor,
              snaking_wt_factor,
              max_move,
              std::move(evaluator),
              logger)
{
}

// Implement the greedy refinement pass
// Different from the FM refinement, greedy refinement
// only accepts possible gain
float GreedyRefine::Pass(
    const HGraphPtr& hgraph,
    const MATRIX<float>& upper_block_balance,
    const MATRIX<float>& lower_block_balance,
    MATRIX<float>& block_balance,        // the current block balance
    MATRIX<int>& net_degs,               // the current net degree
    std::vector<float>& cur_paths_cost,  // the current path cost
    Partitions& solution,
    std::vector<bool>& visited_vertices_flag)
{
  float total_gain = 0.0;  // total gain improvement
  int num_move = 0;
  for (int hyperedge_id = 0; hyperedge_id < hgraph->num_hyperedges_;
       hyperedge_id++) {
    // check if the hyperedge is a straddled_hyperedge
    std::set<int> block_set;
    for (int block_id = 0; block_id < num_parts_; block_id++) {
      if (net_degs[hyperedge_id][block_id] > 0) {
        block_set.insert(block_id);
      }
    }
    // ignore the hyperedge if it's fully within one block
    if (block_set.size() <= 1) {
      continue;
    }
    // updated the iteration
    num_move++;
    if (num_move >= max_move_) {
      return total_gain;
    }
    // find the best candidate block
    // the initialization of best_gain is 0.0
    // define a lambda function to compare two GainHyperedge (>=)
    auto CompareGainHyperedge
        = [&](const GainHyperedge& a, const GainHyperedge& b) {
            if (a->GetGain() > b->GetGain()) {
              return true;
            }
            if (a->GetGain() == b->GetGain()
                && evaluator_->CalculateHyperedgeVertexWtSum(a->GetHyperedge(),
                                                             hgraph)
                       < evaluator_->CalculateHyperedgeVertexWtSum(
                           b->GetHyperedge(), hgraph)) {
              return true;  // break ties based on vertex weight summation of
                            // the hyperedge
            }
            return false;
          };

    // int best_candidate_block = -1;
    auto best_gain_hyperedge = std::make_shared<HyperedgeGain>();
    best_gain_hyperedge->SetGain(0.0f);  // we only accept positive gain
    for (int to_pid = 0; to_pid < num_parts_; to_pid++) {
      if (CheckHyperedgeMoveLegality(hyperedge_id,
                                     to_pid,
                                     hgraph,
                                     solution,
                                     block_balance,
                                     upper_block_balance,
                                     lower_block_balance)
          == true) {
        GainHyperedge gain_hyperedge = CalculateHyperedgeGain(
            hyperedge_id, to_pid, hgraph, solution, cur_paths_cost, net_degs);
        if (best_gain_hyperedge->GetHyperedge() < 0
            || CompareGainHyperedge(gain_hyperedge, best_gain_hyperedge)
                   == true) {
          // best_candidate_block = to_pid;
          best_gain_hyperedge = gain_hyperedge;
        }
      }
    }

    // We only accept positive move
    if (best_gain_hyperedge->GetDestinationPart() > -1
        && best_gain_hyperedge->GetGain() >= 0.0f) {
      // if (best_candidate_block > -1) {
      AcceptHyperedgeGain(best_gain_hyperedge,
                          hgraph,
                          total_gain,
                          solution,
                          cur_paths_cost,
                          block_balance,
                          net_degs);
    }
  }

  // finish traversing all the hyperedges
  return total_gain;
}

}  // namespace par
