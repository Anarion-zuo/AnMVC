//
// Created by anarion on 4/22/20.
//

#ifndef MYMVC_FIXEDSIZECACHE_HPP
#define MYMVC_FIXEDSIZECACHE_HPP

namespace anarion {
    class FixedSizeCache {
    protected:
        LinkedList<Payload*> list;
        size_type maxSize = 1;
    public:
        FixedSizeCache() = default;
        explicit FixedSizeCache(size_type maxSize) : maxSize(maxSize) {}
        FixedSizeCache(const FixedSizeCache &) = default;
        FixedSizeCache(FixedSizeCache &&) noexcept = default;

        void load(Payload *payload) {
            if (list.size() == maxSize) {
                auto it = list.end_iterator();
                --it;
                *it = payload;
                payload->load();
                return;
            }
            list.push_back(payload);
        }
    };
}

#endif //MYMVC_FIXEDSIZECACHE_HPP
