#pragma once

#include "../value/gdsvalue.hh"
#include "gdselement.hh"
#include "gdsboundary.hh"
#include "gdspath.hh"
#include "gdssref.hh"
#include "gdsaref.hh"
#include "gdstext.hh"
#include "gdsnode.hh"
#include "gdsbox.hh"
#include <list>

namespace xtaro::parser
{

    class GDSStructure
    {
    public:
        GDSStructure() noexcept = default;
        ~GDSStructure() noexcept
        {
            delete this->_name;
            delete this->_timestamp;
            
            for (GDSBoundary* boundary : this->_boundarys)
                delete boundary;
            for (GDSPath* path : this->_paths)
                delete path;
            for (GDSStructureReference* sref : this->_srefs)
                delete sref;
            for (GDSArrayReference* aref : this->_arefs)
                delete aref;
            for (GDSText* text : this->_texts)
                delete text;
            for (GDSNode* node : this->_nodes)
                delete node;
            for (GDSBox* box : this->_boxes)
                delete box;
        }

    public:
        GDSString* name() const noexcept
        { return this->_name; }

        std::list<GDSBoundary*>& boundary() noexcept
        { return this->_boundarys; }

        std::list<GDSPath*>& paths() noexcept
        { return this->_paths; }

        std::list<GDSStructureReference*>& structuresReferences() noexcept
        { return this->_srefs; }

        std::list<GDSArrayReference*>& arrayReferences() noexcept
        { return this->_arefs; }

        std::list<GDSText*>& texts() noexcept
        { return this->_texts; }

        std::list<GDSNode*> nodes() noexcept
        { return this->_nodes; }

        std::list<GDSBox*> boxes() noexcept
        { return this->_boxes; }

        void addElements(GDSElement* element)
        {
            switch (element->type())
            {
            case GDSElementType::BOUNDARY:
                this->_boundarys.push_back(dynamic_cast<GDSBoundary*>(element)); break;
            case GDSElementType::PATH:
                this->_paths.push_back(dynamic_cast<GDSPath*>(element)); break;
            case GDSElementType::SREF:
                this->_srefs.push_back(dynamic_cast<GDSStructureReference*>(element)); break;
            case GDSElementType::AREF:
                this->_arefs.push_back(dynamic_cast<GDSArrayReference*>(element)); break;
            case GDSElementType::TEXT:
                this->_texts.push_back(dynamic_cast<GDSText*>(element)); break;
            case GDSElementType::NODE:
                this->_nodes.push_back(dynamic_cast<GDSNode*>(element)); break;
            case GDSElementType::BOX:
                this->_boxes.push_back(dynamic_cast<GDSBox*>(element)); break;
            }
        }

    private:
        GDSString* _name = nullptr;
        GDSTimestamp* _timestamp = nullptr;
        std::list<GDSBoundary*> _boundarys{};
        std::list<GDSPath*> _paths{};
        std::list<GDSStructureReference*> _srefs;
        std::list<GDSArrayReference*> _arefs;
        std::list<GDSText*> _texts;
        std::list<GDSNode*> _nodes;
        std::list<GDSBox*> _boxes;
    };

};