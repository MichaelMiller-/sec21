import {useEffect, useState} from "react";
import {Button, Col, Form, Modal, Row} from "react-bootstrap";
import {DbPointSupport, DbStructuralPoint, SupportTranslation, SupportType} from "./DatabaseInterface";
import axios from "axios";
import {FontAwesomeIcon} from "@fortawesome/react-fontawesome";
import {faThumbtack} from "@fortawesome/free-solid-svg-icons";

type DialogAddPointSupportProps = {
    projectId: string;
    show: boolean;
    onHide: () => void;
    onSubmit: () => void;
}

const DialogAddPointSupport = (props: DialogAddPointSupportProps) => {

    const [data, setData] = useState<DbPointSupport>({
        id: 0,
        uuid: "",
        name: "",
        type: SupportType.FIXED,
        ux: SupportTranslation.FREE,
        uy: SupportTranslation.FREE,
        uz: SupportTranslation.FREE,
        referencePoint: {
            id: 0,
            name: "",
            uuid: "",
            coordinate_x: 0,
            coordinate_y: 0,
            coordinate_z: 0,
        }
    });
    const [structurePointList, setStructurePointList] = useState<DbStructuralPoint[]>([]);
    const [selectedIndexNode, setSelectedIndexNode] = useState(0);
    const [pinned, setPinned] = useState(false);
    const [selectedIndexUx, setSelectedIndexUx] = useState(0);
    const [selectedIndexUy, setSelectedIndexUy] = useState(0);
    const [selectedIndexUz, setSelectedIndexUz] = useState(0);

    const handleInputChange = (e: { target: { name: any; value: any; }; }) => {
        const {name, value} = e.target;
        setData({...data, [name]: value,});
    };

    const handleSubmit = async (event: { currentTarget: any; preventDefault: () => void; stopPropagation: () => void; }) => {
        event.preventDefault();

        // map selected index to enum
        const keys = Object.keys(SupportTranslation);
        data.ux = SupportTranslation[keys[selectedIndexUx] as keyof typeof SupportTranslation];
        data.uy = SupportTranslation[keys[selectedIndexUy] as keyof typeof SupportTranslation];
        data.uy = SupportTranslation[keys[selectedIndexUz] as keyof typeof SupportTranslation];

        await axios.post(process.env.REACT_APP_BACKEND + '/pointsupport/' + structurePointList[selectedIndexNode].id, data)
        props.onSubmit();
        if (pinned === false)
            props.onHide();
    };

    useEffect(() => {
        axios.get(process.env.REACT_APP_BACKEND + '/structuralpoints/' + props.projectId)
            .then(response => setStructurePointList(response.data))
    }, [props.show, props.projectId]);

    return (
        <Modal {...props} size="lg" aria-labelledby="contained-modal-title-vcenter" centered>
            <Modal.Header closeButton>
                <Modal.Title id="contained-modal-title-vcenter">Add new point support</Modal.Title>
            </Modal.Header>
            <Form onSubmit={handleSubmit}>
                <Modal.Body>
                    <Row>
                        <Col xs={7}>
                            <Form.Group controlId="validationPointSupportName">
                                <Form.Label>Name</Form.Label>
                                <Form.Control
                                    required
                                    type="text"
                                    name="name"
                                    value={data.name}
                                    onChange={handleInputChange}
                                />
                                <Form.Text id="nameHelpBlock" muted>
                                    Name of the Support
                                </Form.Text>
                                <Form.Control.Feedback>Please provide a valid name.</Form.Control.Feedback>
                            </Form.Group>
                        </Col>
                        <Col>
                            <Form.Group controlId="controlSelectSupported">
                                <Form.Label>attached</Form.Label>
                                <Form.Control as="select" onChange={(e: any) => {
                                    setSelectedIndexNode(e.target.selectedIndex);
                                }}>
                                    {structurePointList.map(e => (<option key={e.id}>{e.name}</option>))}
                                </Form.Control>
                                <Form.Text id="selectSupportedNodeBlock" muted>
                                    Attached to Node
                                </Form.Text>
                            </Form.Group>
                        </Col>
                    </Row>
                    <Row>
                        <Col>
                            <Form.Label>ux</Form.Label>
                            <Form.Control as="select" onChange={(e: any) => { setSelectedIndexUx(e.target.selectedIndex); }}>
                                { /* TODO: use Object.values(SupportTranslation).map(....); */ }
                                <option key={0}>rigid</option>
                                <option key={1}>free</option>
                            </Form.Control>
                        </Col>
                        <Col>
                            <Form.Label>uy</Form.Label>
                            <Form.Control as="select" onChange={(e: any) => { setSelectedIndexUy(e.target.selectedIndex); }}>
                                { /* TODO: use Object.values(SupportTranslation).map(....); */ }
                                <option key={0}>rigid</option>
                                <option key={1}>free</option>
                            </Form.Control>
                        </Col>
                        <Col>
                            <Form.Label>uz</Form.Label>
                            <Form.Control as="select" onChange={(e: any) => { setSelectedIndexUz(e.target.selectedIndex); }}>
                                { /* TODO: use Object.values(SupportTranslation).map(....); */ }
                                <option key={0}>rigid</option>
                                <option key={1}>free</option>
                            </Form.Control>
                        </Col>
                    </Row>
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

export default DialogAddPointSupport;
