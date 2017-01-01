////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Project:  Embedded Learning Library (ELL)
//  File:     ModelTransformer.h (model)
//  Authors:  Chuck Jacobs
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "InputPort.h"
#include "Model.h"
#include "Node.h"
#include "OutputPort.h"
#include "Port.h"
#include "PortElements.h"

// utilities
#include "Exception.h"

// stl
#include <exception>
#include <functional>
#include <memory>
#include <unordered_map>
#include <vector>

namespace ell
{
/// <summary> model namespace </summary>
namespace model
{
    class InputNodeBase;
    template <typename ValueType>
    class InputNode;

    /// <summary> An action to perform on a node during transformation (refinement/compilation)
    enum class NodeAction
    {
        defaultAction,
        refine,
        compile        
    };

    /// <summary> A function that determines how to process a node </summary>
    typedef std::function<NodeAction(const Node&)> NodeActionFunction;

    /// <summary> A context object that carries information about the compiler or other process driving the transformation. </summary>
    class TransformContext
    {
    public:
        /// <summary> Default Constructor. </summary>
        TransformContext();

        /// <summary> Constructor </summary>
        ///
        /// <param name='nodeActionFunction'> A function that indicates how to override the default refinement or compilation of a node </param>
        TransformContext(const NodeActionFunction& nodeActionFunction);

        /// <summary> Indicates if a node is compilable. </summary>
        ///
        /// <param name="node"> A node. </param>
        /// <returns> Returns true if the node is compilable. </returns>
        bool IsNodeCompilable(const Node& node) const;

        /// <summary> Adds a custom node action function to call during refinement </summary>
        ///
        /// <param name='nodeActionFunction'> A function that indicates how to override the default refinement or compilation of a node </param>
        void SetNodeActionFunction(const NodeActionFunction& nodeActionFunction);

        /// <summary> Gets the action to take on the node during refinement </summary>
        ///
        /// <param name="node"> A node. </param>
        /// <returns> A `NodeAction` enum indicating what action to take on the node </returns>
        NodeAction GetNodeAction(const Node& node) const;

    private:
        NodeActionFunction _nodeActionFunction;
    };

    /// <summary> A class that refines or copies models </summary>
    class ModelTransformer
    {
    public:
        /// <summary> Returns a copy of the input model, by calling Copy() on each of the model's nodes </summary>
        ///
        /// <param name="model"> The model. </param>
        /// <param name="context"> The context. </param>
        ///
        /// <returns> The copied Model. </returns>
        Model CopyModel(const Model& model, const TransformContext& context);

        /// <summary>
        /// Returns a copy of a subset of the input model, by calling Copy() on each of the model's nodes. The
        /// model returned contains the nodes sufficient to compute the given output.
        /// </summary>
        ///
        /// <param name="model"> The model. </param>
        /// <param name="outputNode"> The output node we are interested in </param>
        /// <param name="context"> The context. </param>
        /// <param name="outputNode"> The output that must be computable in the result model </param>
        ///
        /// <returns> The copied model. </returns>
        Model CopyModel(const Model& model, const Node* outputNode, const TransformContext& context);

        /// <summary>
        /// Returns a copy of a subset of the input model, by calling Copy() on each of the model's nodes. The
        /// model returned contains the nodes sufficient to compute the given output.
        /// </summary>
        ///
        /// <param name="model"> The model. </param>
        /// <param name="outputNodes"> The output nodes we are interested in </param>
        /// <param name="context"> The context. </param>
        /// <param name="outputNodes"> The outputs that must be computable in the result model </param>
        ///
        /// <returns> The copied Model. </returns>
        Model CopyModel(const Model& model, const std::vector<const Node*>& outputNodes, const TransformContext& context);

        /// <summary>
        /// Performs one or more refinement iterations on a given model and returns the result.
        /// If context.IsNodeCompilable is not set, this call performs one refinement iteration. If
        /// context.IsNodeCompilable is set, this call refines the model until all its nodes are
        /// compilable or until none of the nodes refine themselves.
        /// </summary>
        ///
        /// <param name="model"> The model. </param>
        /// <param name="context"> The context. </param>
        /// <param name="maxIterations"> The maximum number of refinement iterations. </param>
        ///
        /// <returns> The refined Model. </returns>
        Model RefineModel(const Model& model, const TransformContext& context, int maxIterations = 10);

        /// <summary> Transforms the model by applying a transformation function to each node </summary>
        ///
        /// <param name="model"> The model to transform. </param>
        /// <param name="transformFunction"> The function to apply on each node </param>
        /// <param name="context"> The TransformContext to use during the transformation </param>
        ///
        /// <returns> The refined Model. </returns>
        Model TransformModel(const Model& model, const std::function<void(const Node&, ModelTransformer&)>& transformFunction, const TransformContext& context);

        /// <summary> Indicates if the last call to RefineModel produced a model that is compilable. </summary>
        ///
        /// <returns> true if the model returned by RefineModel is compilable. </returns>
        /// <remarks> Only available after calling CopyModel or RefineModel. </remarks>
        bool IsModelCompilable() const { return _isModelCompilable; }

        /// <summary> Returns the port elements from the new model corresponding to the given port on the input model </summary>
        /// <remarks> Only available after calling CopyModel or RefineModel </remarks>
        template <typename ValueType>
        PortElements<ValueType> GetCorrespondingOutputs(const OutputPort<ValueType>& port);

        /// <summary> Returns the port elements from the new model corresponding to the given port on the input model </summary>
        /// <remarks> Only available after calling CopyModel or RefineModel </remarks>
        PortElementsBase GetCorrespondingOutputs(const OutputPortBase& port);

        /// <summary> Returns the port elements from the new model corresponding to the given elements on the input model </summary>
        /// <remarks> Only available after calling CopyModel or RefineModel </remarks>
        template <typename ValueType>
        PortElements<ValueType> GetCorrespondingOutputs(const PortElements<ValueType>& elements);

        /// <summary> Returns the port elements from the new model corresponding to the given elements on the input model </summary>
        /// <remarks> Only available after calling CopyModel or RefineModel </remarks>
        PortElementsBase GetCorrespondingOutputs(const PortElementsBase& elements);

        /// <summary> Returns the input node from new new model corresponding to the given input node on the input model </summary>
        /// <remarks> Only available after calling CopyModel or RefineModel </remarks>
        template <typename ValueType>
        InputNode<ValueType>* GetCorrespondingInputNode(const InputNode<ValueType>* node);

        /// <summary> Returns the input node from new new model corresponding to the given input node on the input model </summary>
        /// <remarks> Only available after calling CopyModel or RefineModel </remarks>
        InputNodeBase* GetCorrespondingInputNode(const InputNodeBase* node);

        ///
        /// Functions used by node implementors
        ///

        /// <summary> Transforms a set of output port references from the input model space to the output model space. Called by node implementors. </summary>
        ///
        /// <param name="elements"> The elements in the input model to transform to the output model space. </param>
        /// <returns> A `PortElements` object representing the transformed elements in the space of the new model. </returns>
        template <typename ValueType>
        PortElements<ValueType> TransformPortElements(const PortElements<ValueType>& elements);

        /// <summary> Transforms a set of output port references from the input model space to the output model space. Called by node implementors. </summary>
        ///
        /// <param name="elements"> The elements in the input model to transform to the output model space. </param>
        /// <returns> A `PortElementsBase` object representing the transformed elements in the space of the new model. </returns>
        PortElementsBase TransformPortElements(const PortElementsBase& elements);

        /// <summary> Creates a new node in the transformed model. Called by node implementors. </summary>
        ///
        /// <typeparam name="Args"> The arguments to the constructor of NodeType. </typeparam>
        template <typename NodeType, typename... Args>
        NodeType* AddNode(Args&&... args);

        /// <summary> Sets up an old-to-new model output mapping. Called by node implementors </summary>
        ///
        /// <param name="oldPort"> The port in the old model to map to the new model. </param>
        /// <param name="newPort"> The port in the new model to be mapped from the old model. </param>
        template <typename ValueType>
        void MapNodeOutput(const OutputPort<ValueType>& oldPort, const OutputPort<ValueType>& newPort);

        /// <summary> Sets up an old-to-new model output mapping. Called by node implementors </summary>
        ///
        /// <param name="oldPort"> The port in the old model to map to the new model. </param>
        /// <param name="newPort"> The port in the new model to be mapped from the old model. </param>
        template <typename ValueType>
        void MapNodeOutput(const OutputPort<ValueType>& oldPort, const PortElements<ValueType>& newElements);

        /// <summary> Sets up an old-to-new model output mapping. Called by node implementors </summary>
        ///
        /// <param name="oldElements"> The elements in the old model to map to the new model. </param>
        /// <param name="newElements"> The elements in the new model to be mapped from the old model. </param>
        template <typename ValueType>
        void MapNodeOutput(const PortElements<ValueType>& oldElements, const PortElements<ValueType>& newElements);

        /// <summary> Get the context used by the transformer. Called by node implementors </summary>
        ///
        /// <returns> The context in use by the transformer. </returns>
        TransformContext& GetContext() { return _context; }

        /// <summary> Gets the underlying model. </summary>
        ///
        /// <returns> The model. </returns>
        Model& GetModel() { return _model; }

    private:
        friend class Node;

        template <typename NodeType>
        NodeType* GetCorrespondingInputNodeAs(const NodeType* node);

        // Find a node that isn't compilable (if there are several, it just finds one)
        std::vector<const Node*> FindUncompilableNodes(const Model& model, const TransformContext& context) const;

        Model _model;
        TransformContext _context;
        std::unordered_map<PortElementBase, PortElementBase> _elementToElementMap;
        bool _isModelCompilable;
    };
}
}

#include "../tcc/ModelTransformer.tcc"
