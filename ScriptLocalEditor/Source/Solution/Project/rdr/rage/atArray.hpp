#pragma once

namespace rage
{
#pragma pack(push, 8)
    template <typename T>
    class atArray
    {
    public:
        atArray()
        {
            m_data = nullptr;
            m_count = 0;
            m_size = 0;
        }

        T* begin() const
        {
            return &m_data[0];
        }

        T* end() const
        {
            return &m_data[m_size];
        }

        T* data() const
        {
            return m_data;
        }

        std::uint16_t size() const
        {
            return m_size;
        }

        std::uint16_t count() const
        {
            return m_count;
        }

        T& operator[](std::uint16_t index) const
        {
            return m_data[index];
        }

    private:
        T* m_data;
        std::uint16_t m_size;
        std::uint16_t m_count;
    };
    static_assert(sizeof(rage::atArray<uint32_t>) == 0x10, "rage::atArray is not properly sized");
#pragma pack(pop)
}