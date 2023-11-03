#include <iostream>
#include <vector>
#include <map>
#include <assert.h>


struct nonLeafValue
{
    unsigned int beginKey;
    unsigned int endKey;
    unsigned int pageID;
};

struct leafValue
{
    unsigned int key;
};

class PageHeader
{
public:
    PageHeader(
        unsigned int pageID,
        unsigned int level)
        :
        m_pageID(pageID),
        m_level(level),
        m_slotCount(0),
        m_endKey(0)
    {}
protected:
    unsigned int m_pageID;

    // 0 is leaf.
    //
    unsigned int m_level;

    unsigned int m_slotCount;

    unsigned int m_endKey;
};

const int PAGE_SIZE = 8192;
const int PAGE_DATA_SIZE = PAGE_SIZE - sizeof(PageHeader);

class Page : public PageHeader
{
public:
    Page(
        unsigned int pageID,
        unsigned int level)
        :
        PageHeader(pageID, level)
    {
        memset(m_data, 0, (unsigned int)(PAGE_DATA_SIZE));
    }

    unsigned int GetPageId()
    {
        return m_pageID;
    }

    bool IsLeafLevel()
    {
        return m_level == 0;
    }

    void InsertIntoLeaf()
    {
        if (sizeof(unsigned int) * (m_slotCount + 1) >= PAGE_DATA_SIZE)
        {
            assert(0);
        }

        m_slotCount++;
        m_endKey++;
        *((unsigned int*)(m_data)+m_slotCount) = (unsigned int)(m_endKey);
    }

    void PrintPage()
    {
        for (unsigned int i = 0; i < m_slotCount; i++)
        {
            std::cout << *((unsigned int*)(m_data)+i) << std::endl;
        }
    }

private:
    unsigned char m_data[PAGE_DATA_SIZE];
};

class BufferPool
{
public:
    BufferPool()
        :
        m_nextPageID(0)
    {}

    Page* GetNewPage(
        unsigned int level)
    {
        Page* page = new Page(m_nextPageID, level);
        m_pages[m_nextPageID] = page;
        m_nextPageID++;

        return page;
    }

    Page* FindPage(
        unsigned int pageID)
    {
        return m_pages[pageID];
    }

private:
    unsigned int m_nextPageID;
    std::map<int, Page*> m_pages;
};

BufferPool bufferPool;

class BTree
{
public:
    BTree()
    {
        m_rootLevel = 0;
        m_rootPageID = bufferPool.GetNewPage(m_rootLevel)->GetPageId();
    }

    void Insert()
    {
        Page* rootPage = bufferPool.FindPage(m_rootPageID);

        if (rootPage->IsLeafLevel())
        {
            rootPage->InsertIntoLeaf();
        }
    }

    void SelectStar()
    {
        Page* rootPage = bufferPool.FindPage(m_rootPageID);

        if (rootPage->IsLeafLevel())
        {
            rootPage->PrintPage();
        }
    }

private:
    unsigned int m_rootPageID;
    unsigned int m_rootLevel;
};

int main() {
    BTree btree;

    for (int i = 0; i < 20; i++)
    {
        btree.Insert();
    }

    btree.SelectStar();

    return 0;
}