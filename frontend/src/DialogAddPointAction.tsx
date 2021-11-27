import {useEffect, useState} from "react";
import {Button, Col, Form, Modal} from "react-bootstrap";
import {ActionType, DbLoadCase, DbPointAction, DbStructuralPoint, LoadType} from "./DatabaseInterface";
import axios from "axios";
import {FontAwesomeIcon} from "@fortawesome/react-fontawesome";
import {faThumbtack} from "@fortawesome/free-solid-svg-icons";

type DialogAddPointActionProps = {
    projectId: string;
    show: boolean;
    onHide: () => void;
    onSubmit: () => void;
}

const DialogAddPointAction = (props: DialogAddPointActionProps) => {

    const [data, setData] = useState<DbPointAction>({
        id: 0,
        uuid: "",
        name: "",
        value_x: 0,
        value_y: 0,
        value_z: 0,
        loadcase: {
            id: 0,
            uuid: "",
            name: "",
            description: "",
            actionType: ActionType.PERMANENT,
            type: LoadType.STANDARD
        },
        referencePoint: {
            id: 0,
            name: "",
            uuid: "",
            coordinate_x: 0,
            coordinate_y: 0,
            coordinate_z: 0,
        }
    });
    const [loadcaseList, setLoadcaseList] = useState<DbLoadCase[]>([]);
    const [structurePointList, setStructurePointList] = useState<DbStructuralPoint[]>([]);
    const [pinned, setPinned] = useState(false);
    const [selectedLoadCase, setSelectedLoadCase] = useState(0);
    const [selectedRefPoint, setSelectedRefPoint] = useState(0);

    const handleInputChange = (e: { target: { name: any; value: any; }; }) => {
        const {name, value} = e.target;
        setData({...data, [name]: value,});
    };

    const handleSubmit = async (event: { currentTarget: any; preventDefault: () => void; stopPropagation: () => void; }) => {
        event.preventDefault();
        await axios.post(process.env.REACT_APP_BACKEND + '/pointaction/' + loadcaseList[selectedLoadCase].id + '/' + structurePointList[selectedRefPoint].id, data)
        props.onSubmit();
        if (pinned === false)
            props.onHide();
    };

    useEffect(() => {
        axios.get(process.env.REACT_APP_BACKEND + '/loadcases/' + props.projectId)
            .then(response => setLoadcaseList(response.data))
        axios.get(process.env.REACT_APP_BACKEND + '/structuralpoints/' + props.projectId)
            .then(response => setStructurePointList(response.data))
    }, [props.show, props.projectId]);

    return (
        <Modal {...props} size="lg" aria-labelledby="contained-modal-title-vcenter" centered>
            <Modal.Header closeButton>
                <Modal.Title id="contained-modal-title-vcenter">Add new load case</Modal.Title>
            </Modal.Header>
            <Form onSubmit={handleSubmit}>
                <Modal.Body>
                    <Form.Group as={Col} md="4" controlId="validationPointActionName">
                        <Form.Label>Name</Form.Label>
                        <Form.Control
                            required
                            type="text"
                            name="name"
                            value={data.name}
                            onChange={handleInputChange}
                        />
                        <Form.Control.Feedback>Please provide a valid name.</Form.Control.Feedback>
                    </Form.Group>
                    <Form.Group controlId="controlSelectLoadCase">
                        <Form.Label>Loadcase</Form.Label>
                        <Form.Control as="select" onChange={(e: any) => {
                            setSelectedLoadCase(e.target.selectedIndex);
                        }}>
                            {loadcaseList.map(e => (<option key={e.id}>{e.name}</option>))}
                        </Form.Control>
                    </Form.Group>
                    <Form.Group as={Col} md="4" controlId="validationPointActionValueX">
                        <Form.Label>Value in X [N]</Form.Label>
                        <Form.Control
                            required
                            type="text"
                            name="value_x"
                            placeholder="Value in X [N]"
                            value={data.value_x}
                            onChange={handleInputChange}
                        />
                        <Form.Control.Feedback>Please provide a valid value.</Form.Control.Feedback>
                    </Form.Group>
                    <Form.Group as={Col} md="4" controlId="validationPointActionValueY">
                        <Form.Label>Value in Y [N]</Form.Label>
                        <Form.Control
                            required
                            type="text"
                            name="value_y"
                            placeholder="Value in Y [N]"
                            value={data.value_y}
                            onChange={handleInputChange}
                        />
                        <Form.Control.Feedback>Please provide a valid value.</Form.Control.Feedback>
                    </Form.Group>
                    <Form.Group as={Col} md="4" controlId="validationPointActionValueZ">
                        <Form.Label>Value in Z [N]</Form.Label>
                        <Form.Control
                            required
                            type="text"
                            name="value_z"
                            placeholder="Value in Z [N]"
                            value={data.value_z}
                            onChange={handleInputChange}
                        />
                        <Form.Control.Feedback>Please provide a valid value.</Form.Control.Feedback>
                    </Form.Group>
                    <Form.Group controlId="controlSelectRefNode">
                        <Form.Label>from</Form.Label>
                        <Form.Control as="select" onChange={(e: any) => {
                            setSelectedRefPoint(e.target.selectedIndex);
                        }}>
                            {structurePointList.map(e => (<option key={e.id}>{e.name}</option>))}
                        </Form.Control>
                    </Form.Group>
                </Modal.Body>
                <Modal.Footer>
                    <Button variant="link" onClick={() => {
                        setPinned(!pinned)
                    }}>
                        <FontAwesomeIcon icon={faThumbtack} flip={pinned ? "vertical" : "horizontal"} size="2x"/>
                    </Button>
                    <Button type="submit">Submit</Button>
                    <Button onClick={props.onHide}>Close</Button>
                </Modal.Footer>
            </Form>
        </Modal>
    );
}

export default DialogAddPointAction;
