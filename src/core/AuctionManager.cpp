#include "core/AuctionManager.hpp"

AuctionManager::AuctionManager()
    : property(nullptr),
      highestBid(0),
      highestBidder(nullptr),
      consecutivePasses(0) {}
